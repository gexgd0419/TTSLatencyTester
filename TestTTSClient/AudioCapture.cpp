#define WIN32_LEAN_AND_MEAN
#include <mmdeviceapi.h>
#include <Audioclient.h>
#pragma warning(suppress : 4996) // 'GetVersionExW' deprecated
#include <sphelper.h>
#include <atlcomcli.h>
#include <mmsystem.h>
#include <system_error>
#include <iostream>
#include <iomanip>
#include <thread>
#include <fstream>
#include <string>

using ATL::CComPtr;

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

static inline void CheckHr(HRESULT hr)
{
    if (FAILED(hr))
        throw std::system_error(hr, std::system_category());
}

static DWORD64 GetPerfFreq()
{
    LARGE_INTEGER perfFreq;
    QueryPerformanceFrequency(&perfFreq);
    return perfFreq.QuadPart;
}

static DWORD64 s_perfCounterFreq = GetPerfFreq();

// return performance counter in 100ns unit
static DWORD64 GetPerfCounter()
{
    LARGE_INTEGER perfCount;
    QueryPerformanceCounter(&perfCount);
    DWORD64 cnt = perfCount.QuadPart;

    // on x86 systems, frequency is usually 10 MHz, which is exactly REFTIMES_PER_SEC Hz
    if (s_perfCounterFreq == REFTIMES_PER_SEC)
        return cnt;

    // do (cnt * REFTIMES_PER_SEC / s_perfCounterFreq), but prevent overflow
    DWORD64 whole = (cnt / s_perfCounterFreq) * REFTIMES_PER_SEC;
    DWORD64 part  = (cnt % s_perfCounterFreq) * REFTIMES_PER_SEC / s_perfCounterFreq;
    return whole + part;
}

template <typename SampleType, size_t bytesPerSample>
constexpr SampleType GetSilencePoint()
{
    // for unsigned types, zero point is at middle
    // for signed types, zero is zero
    if constexpr (std::is_unsigned_v<SampleType>)
        return SampleType(1) << (bytesPerSample * 8 - 1);
    else
        return SampleType();
}

template <typename SampleType, size_t bytesPerSample>
constexpr SampleType GetThreshold()
{
    // Threshold: 1 / 2^10 or 0.0009765625
    if constexpr (std::is_floating_point_v<SampleType>)
        return SampleType(1) / (1 << 10);
    else if constexpr (bytesPerSample * 8 > 10)
        return SampleType(1) << (bytesPerSample * 8 - 10);
    else
        return SampleType();
}

static WORD GetFormatTag(const WAVEFORMATEX* wfx)
{
    if (wfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE)
    {
        auto wfext = reinterpret_cast<const WAVEFORMATEXTENSIBLE*>(wfx);
        if (IS_VALID_WAVEFORMATEX_GUID(&wfext->SubFormat))
            return EXTRACT_WAVEFORMATEX_ID(&wfext->SubFormat);
    }
    return wfx->wFormatTag;
}

// Returns the leading silence wave data length, in bytes
template <typename SampleType, size_t bytesPerSample = sizeof(SampleType)>
size_t GetLeadingSilenceLengthMono(const BYTE* waveData, size_t length)
{
    if (length < bytesPerSample)
        return 0;

    constexpr SampleType silencePoint = GetSilencePoint<SampleType, bytesPerSample>();
    constexpr SampleType threshold = GetThreshold<SampleType, bytesPerSample>();
    constexpr SampleType minValue = silencePoint - threshold, maxValue = silencePoint + threshold;

    // Check each sample
    const BYTE* p = waveData;
    const BYTE* pEnd = waveData + (length - (length % bytesPerSample));
    SampleType smp;
    for (; p != pEnd; p += bytesPerSample)
    {
        if constexpr (bytesPerSample < sizeof(SampleType))
            smp = SampleType();
        memcpy(&smp, p, bytesPerSample);
        // this sample is out of range, so the leading silence starts at the previous sample
        if (smp < minValue || smp > maxValue)
            return p - waveData;
    }

    // The whole data block is silence
    return length;
}

static size_t GetLeadingSilenceLength(const WAVEFORMATEX* wfx, const BYTE* waveData, size_t length)
{
    size_t len;

    switch (GetFormatTag(wfx))
    {
    case WAVE_FORMAT_PCM:
        switch (wfx->wBitsPerSample)
        {
        case 8:  // 8-bits are unsigned, others are signed
            len = GetLeadingSilenceLengthMono<BYTE>(waveData, length);
            break;
        case 16:
            len = GetLeadingSilenceLengthMono<SHORT>(waveData, length);
            break;
        case 24:
            len = GetLeadingSilenceLengthMono<LONG, 3>(waveData, length);
            break;
        case 32:
            len = GetLeadingSilenceLengthMono<LONG>(waveData, length);
            break;
        default:
            return 0;
        }
        break;
    case WAVE_FORMAT_IEEE_FLOAT:
        switch (wfx->wBitsPerSample)
        {
        case 32:
            len = GetLeadingSilenceLengthMono<float>(waveData, length);
            break;
        case 64:
            len = GetLeadingSilenceLengthMono<double>(waveData, length);
            break;
        default:
            return 0;
        }
        break;
    default:
        return 0;
    }

    return len - len % wfx->nBlockAlign;  // round down to block (channel) boundaries
}

static CComPtr<IMMDevice> OpenDefaultDevice(EDataFlow dataFlow)
{
    CComPtr<IMMDeviceEnumerator> enumerator;
    CheckHr(enumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator)));
    CComPtr<IMMDevice> dev;
    CheckHr(enumerator->GetDefaultAudioEndpoint(dataFlow, eConsole, &dev));
    return dev;
}

static bool s_cancelled = false;
static HWND msgwnd = nullptr;

#define WM_AUDIORECEIVED WM_APP

static void AudioCapture()
{
    auto dev = OpenDefaultDevice(eRender);

    CComPtr<IAudioClient> audclient;
    CheckHr(dev->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&audclient)));

    CSpCoTaskMemPtr<WAVEFORMATEX> pwfx;
    CheckHr(audclient->GetMixFormat(&pwfx));
    const DWORD bytesPerFrame = pwfx->nBlockAlign;
    const DWORD framesPerSecond = pwfx->nSamplesPerSec;

    CheckHr(audclient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK,
        REFTIMES_PER_MILLISEC * 100,  // 100ms buffer
        0, pwfx, nullptr));

    CComPtr<IAudioCaptureClient> capclient;
    CheckHr(audclient->GetService(__uuidof(IAudioCaptureClient), reinterpret_cast<void**>(&capclient)));

    CheckHr(audclient->Start());

    DWORD64 lastEndTime = 0;
    bool hasSound = false;
    while (!s_cancelled)
    {
        PBYTE pData;
        UINT32 framesRead;
        DWORD flags;
        DWORD64 timeAtFirstFrame;
        HRESULT hr = capclient->GetBuffer(&pData, &framesRead, &flags, nullptr, &timeAtFirstFrame);
        CheckHr(hr);
        if (hr != S_OK)
        {
            if (hasSound && lastEndTime - GetPerfCounter() > REFTIMES_PER_MILLISEC * 100)
                hasSound = false;
            Sleep(10);
            continue;
        }
        if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
        {
            if (hasSound && lastEndTime - GetPerfCounter() > REFTIMES_PER_MILLISEC * 100)
                hasSound = false;
            CheckHr(capclient->ReleaseBuffer(framesRead));
            continue;
        }

        const size_t dataLen = bytesPerFrame * framesRead;
        const size_t silenceLen = GetLeadingSilenceLength(pwfx, pData, dataLen);
        CheckHr(capclient->ReleaseBuffer(framesRead));
        if (silenceLen == dataLen)
        {
            if (hasSound && lastEndTime - GetPerfCounter() > REFTIMES_PER_MILLISEC * 100)
                hasSound = false;
            continue;
        }

        const DWORD64 silenceDuration = static_cast<DWORD64>(silenceLen / bytesPerFrame) * REFTIMES_PER_SEC / framesPerSecond;
        const DWORD64 detectedTime = timeAtFirstFrame + silenceDuration;  // Performance Counter time in 100ns when audio is detected

        if (!hasSound)
        {
            PostMessageW(msgwnd, WM_AUDIORECEIVED, LODWORD(detectedTime), HIDWORD(detectedTime));
            hasSound = true;
        }
        lastEndTime = timeAtFirstFrame + (DWORD64)framesRead * REFTIMES_PER_SEC / framesPerSecond;
    }
}

static void AudioCaptureThread()
{
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    try
    {
        CheckHr(CoInitialize(nullptr));
        AudioCapture();
    }
    catch (const std::system_error& ex)
    {
        std::cerr << "Error " << "0x" << std::hex << (UINT)ex.code().value() << ": " << ex.what() << '\n';
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << '\n';
    }
    CoUninitialize();
}

extern bool g_outputCSVFile;
static std::wofstream s_file;

static LRESULT CALLBACK RawInputWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static DWORD64 lastKeyTime = 0;
    static bool waitingForAudio = false;

    if (message == WM_INPUT)
    {
        DWORD64 now = GetPerfCounter();
        DWORD delayMs = GetTickCount() - (DWORD)GetMessageTime();
        DWORD64 time = now + (DWORD64)delayMs * REFTIMES_PER_MILLISEC;

        UINT size = 0;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
        std::unique_ptr<BYTE[]> pbuf(new BYTE[size]);
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, pbuf.get(), &size, sizeof(RAWINPUTHEADER));
        RAWINPUT* raw = (RAWINPUT*)pbuf.get();
        if (raw->header.dwType == RIM_TYPEKEYBOARD
            && !(raw->data.keyboard.Flags & RI_KEY_BREAK))  // keyboard key pressed (not released)
        {
            lastKeyTime = time;
            waitingForAudio = true;
        }
        return 0;
    }
    else if (message == WM_AUDIORECEIVED)
    {
        DWORD64 audioTime = wParam | ((DWORD64)lParam << 32);
        if (waitingForAudio && lastKeyTime < audioTime)
        {
            std::wcout
                << std::fixed << std::setprecision(2) << std::setw(10)
                << ((double)(audioTime - lastKeyTime) / REFTIMES_PER_MILLISEC) << L'\n';
            if (g_outputCSVFile)
            {
                s_file
                    << std::fixed << std::setprecision(2)
                    << L'"' << ((double)(audioTime - lastKeyTime) / REFTIMES_PER_MILLISEC) << L"\"\n";
                s_file.flush();
            }
            waitingForAudio = false;
        }
        return 0;
    }
    else if (message == WM_DESTROY)
    {
        PostQuitMessage(0);
    }

    return DefWindowProcW(hWnd, message, wParam, lParam);
}

static BOOL WINAPI CtrlHandler(DWORD type)
{
    if (type == CTRL_C_EVENT || type == CTRL_BREAK_EVENT)
    {
        s_cancelled = true;
        PostMessageW(msgwnd, WM_CLOSE, 0, 0);
        return TRUE;
    }
    return FALSE;
}

void DetectKeypressLatency()
{
    std::wcout << L"This program will start detecting keypresses and audio output.\n"
        "Detected latency (in milliseconds) between keypress and the audio will be listed below.\n"
        "It is recommended to close other apps that can emit audio.\n"
        "There should be enough duration of silence between two keypresses so that they can be distinguished properly.\n"
        "Press Ctrl+C to stop detecting.\n\n";
    std::wcout << std::setw(10) << "Latency/ms\n";

    WNDCLASSEXW wcex = { sizeof wcex };
    wcex.lpfnWndProc = RawInputWndProc;
    wcex.lpszClassName = L"RawInput window";
    RegisterClassExW(&wcex);
    msgwnd = CreateWindowExW(0, L"RawInput window", nullptr, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, nullptr, nullptr);
    if (!msgwnd)
    {
        std::wcerr << L"Cannot create RawInput window\n";
        UnregisterClassW(L"RawInput window", nullptr);
        return;
    }

    RAWINPUTDEVICE rid = { 0 };
    rid.usUsagePage = 1;  // generic
    rid.usUsage = 6;  // keyboard
    rid.dwFlags = RIDEV_INPUTSINK | RIDEV_NOLEGACY;
    rid.hwndTarget = msgwnd;

    if (!RegisterRawInputDevices(&rid, 1, sizeof rid))
    {
        std::wcerr << L"Cannot register raw input devices\n";
        UnregisterClassW(L"RawInput window", nullptr);
        return;
    }

    std::wstring filename;
    if (g_outputCSVFile)
    {
        filename = L"KeyDelayLog_" + std::to_wstring(time(nullptr)) + L".csv";
        s_file.open(filename, std::ios::out | std::ios::trunc);
        s_file << L"\"Delay/ms\"\n";
    }

    auto audioThread = std::thread(AudioCaptureThread);

    SetConsoleCtrlHandler(CtrlHandler, TRUE);

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        DispatchMessageW(&msg);
    }

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
    audioThread.join();
    s_cancelled = false;

    if (g_outputCSVFile)
    {
        if (s_file)
        {
            s_file.close();
            std::wcout << L"Log CSV file stored: " << filename << L"\n\n";
        }
    }

    SetConsoleCtrlHandler(CtrlHandler, FALSE);

    rid.dwFlags = RIDEV_REMOVE;
    rid.hwndTarget = nullptr;
    RegisterRawInputDevices(&rid, 1, sizeof rid);
    DestroyWindow(msgwnd);
    UnregisterClassW(L"RawInput window", nullptr);
}
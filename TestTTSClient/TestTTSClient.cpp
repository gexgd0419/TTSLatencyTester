// TestTTSClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <sapi.h>
#pragma warning(suppress : 4996) // 'GetVersionExW' deprecated
#include <sphelper.h>
#include <atlcomcli.h>

#include "../TestTTSEngine/TestTTSEngine_i.h"

using ATL::CComPtr;

static CComPtr<ISpVoice> spVoice;
static CComPtr<ILogger> logger;
static CComPtr<ISpObjectToken> voiceToken;
static bool outputCSVFile = false;

static inline void CheckHr(HRESULT hr)
{
    if (FAILED(hr))
        throw std::system_error(hr, std::system_category());
}

static void SelectVoice()
{
    for (;;)
    {
        CComPtr<IEnumSpObjectTokens> pEnum;
        CheckHr(SpEnumTokens(SPCAT_VOICES, L"EngineCLSID;TokenId", nullptr, &pEnum));
        std::wcout << L"Select a voice in the following list.\n\n";
        UINT i = 1;
        for (CComPtr<ISpObjectToken> pTok; pEnum->Next(1, &pTok, nullptr) == S_OK; pTok.Release())
        {
            CSpDynamicString desc;
            CheckHr(SpGetDescription(pTok, &desc));
            std::wcout << std::setw(2) << i << L": " << (LPCWSTR)desc << L'\n';
            i++;
        }
        std::wcout << L"\nEnter a number: ";
        std::wcin >> i;
        voiceToken.Release();
        if (pEnum->Item(i - 1, &voiceToken) == S_OK)
        {
            CSpDynamicString desc;
            CheckHr(SpGetDescription(voiceToken, &desc));
            std::wcout << "Voice selected: " << (LPCWSTR)desc << L"\n\n";
            return;
        }
        else
        {
            std::wcout << "Invalid index.\n\n";
        }
    }
}

static void LoadVoice(IUnknown* pOutput = nullptr)
{
    CheckHr(spVoice->SetVoice(voiceToken));
    CComPtr<ISpObjectToken> output;
    CheckHr(spVoice->GetOutputObjectToken(&output));
    CheckHr(spVoice->SetOutput(output, TRUE));  // reset audio
}

static void OutputLogs()
{
    logger->LogEnd();
    CComPtr<ILogItems> items;
    CheckHr(logger->GetLogItems(&items));
    long count;
    CheckHr(items->get_Count(&count));
    double last_ms = 0;
    std::wofstream file;
    std::wstring filename;
    std::wcout << L"---------- Log start ----------\n";
    std::wcout
        << std::setw(10) << L"Total/ms"
        << std::setw(10) << L"Delta/ms"
        << L" Step\n";
    if (outputCSVFile)
    {
        filename = L"TTSLog_" + std::to_wstring(time(nullptr)) + L".csv";
        file.open(filename, std::ios::out | std::ios::trunc);
        file << L"\"Total/ms\",\"Delta/ms\",\"Step\"\n";
    }
    for (long i = 1; i <= count; i++)
    {
        CComPtr<ILogItem> item;
        CheckHr(items->get_Item(i, &item));
        double seconds;
        CComBSTR msg;
        CheckHr(item->get_ElapsedSeconds(&seconds));
        CheckHr(item->get_Message(&msg));
        double ms = seconds * 1000.0;
        std::wcout
            << std::fixed << std::setprecision(2)
            << std::setw(10) << ms
            << std::setw(10) << (ms - last_ms)
            << L' ' << (LPCWSTR)msg << L'\n';
        if (outputCSVFile)
        {
            file
                << std::fixed << std::setprecision(2)
                << L'"' << ms << "\",\"" << (ms - last_ms)
                << "\",\"" << (LPCWSTR)msg << L"\"\n";
        }
        last_ms = ms;
    }
    std::wcout << L"----------- Log end -----------\n";
    if (outputCSVFile)
    {
        if (file)
        {
            file.close();
            std::wcout << L"Log CSV file stored: " << filename << L'\n\n';
        }
        else
        {
            std::wcerr << L"Log CSV file cannot be stored.\n\n";
        }
    }
}

class EventSink : public ISpNotifySink
{
public:
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)
    {
        if (riid == IID_ISpNotifySink || riid == IID_IUnknown)
        {
            *ppvObject = this;
            ++m_ref;
            return S_OK;
        }
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef()
    {
        return ++m_ref;
    }
    ULONG STDMETHODCALLTYPE Release()
    {
        if (--m_ref == 0)
        {
            delete this;
            return 0;
        }
        return m_ref;
    }
    STDMETHODIMP Notify()
    {
        // This can be called on another thread
        CSpEvent evt;
        while (evt.GetFrom(spVoice) == S_OK)
        {
            switch (evt.eEventId)
            {
            case SPEI_START_INPUT_STREAM:
                logger->LogStepC(L"Client StartStream event");
                break;
            case SPEI_END_INPUT_STREAM:
                logger->LogStepC(L"Client EndStream event");
                break;
            }
        }
        return S_OK;
    }

private:
    ULONG m_ref = 0;
};

CComPtr<ISpNotifySink> notifySink(new EventSink);

static void TestSyncSpeak()
{
    std::wcout << L"Performing tests...\n";
    logger->LogStart();
    LoadVoice();

    for (int i = 1; i <= 5; i++)
    {
        logger->LogStepC(L"Client Speak start");
        CheckHr(spVoice->Speak(std::to_wstring(i).c_str(), 0, nullptr));  // speak synchronously
        logger->LogStepC(L"Client Speak end");
    }

    OutputLogs();
}

class AudioStream : public IStream
{
public:
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)
    {
        if (riid == IID_IStream || riid == IID_ISequentialStream || riid == IID_IUnknown)
        {
            *ppvObject = this;
            ++m_ref;
            return S_OK;
        }
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef()
    {
        return ++m_ref;
    }
    ULONG STDMETHODCALLTYPE Release()
    {
        if (--m_ref == 0)
        {
            delete this;
            return 0;
        }
        return m_ref;
    }

    // Only a few methods of IStream are used by SAPI. The rest are left unimplemented.

    STDMETHODIMP Read(void* pv, ULONG cb, ULONG* pcbRead)
    {
        return E_NOTIMPL;
    }
    STDMETHODIMP Write(const void* pv, ULONG cb, ULONG* pcbWritten)
    {
        if (m_cbWritten == 0 && cb > 0)
        {
            logger->LogStepC(L"Client audio data received");
        }
        m_cbWritten += cb;
        if (pcbWritten)
            *pcbWritten = cb;
        return S_OK;
    }
    STDMETHODIMP Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition)
    {
        if (dwOrigin == STREAM_SEEK_CUR && dlibMove.QuadPart == 0)
        {
            // SAPI uses this to get the current position. Other Seek requests are left unimplemented.
            if (plibNewPosition)
                plibNewPosition->QuadPart = m_cbWritten;
            return S_OK;
        }
        return E_NOTIMPL;
    }
    STDMETHODIMP SetSize(ULARGE_INTEGER libNewSize)
    {
        return E_NOTIMPL;
    }
    STDMETHODIMP CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten)
    {
        return E_NOTIMPL;
    }
    STDMETHODIMP Commit(DWORD grfCommitFlags)
    {
        return E_NOTIMPL;
    }
    STDMETHODIMP Revert(void)
    {
        return E_NOTIMPL;
    }
    STDMETHODIMP LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
    {
        return E_NOTIMPL;
    }
    STDMETHODIMP UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
    {
        return E_NOTIMPL;
    }
    STDMETHODIMP Stat(STATSTG* pstatstg, DWORD grfStatFlag)
    {
        return E_NOTIMPL;
    }
    STDMETHODIMP Clone(IStream** ppstm)
    {
        return E_NOTIMPL;
    }
private:
    ULONGLONG m_cbWritten = 0;
    ULONG m_ref = 0;
};

static void TestSyncSpeakStream()
{
    std::wcout << L"Performing tests...\n";
    GUID fmtguid;
    CSpCoTaskMemPtr<WAVEFORMATEX> wfx;
    CheckHr(SpConvertStreamFormatEnum(SPSTREAMFORMAT::SPSF_48kHz16BitMono, &fmtguid, &wfx));
    logger->LogStart();
    LoadVoice();

    for (int i = 1; i <= 5; i++)
    {
        CComPtr<IStream> myStream(new AudioStream);
        CComPtr<ISpStream> spStream;
        CheckHr(spStream.CoCreateInstance(CLSID_SpStream));
        CheckHr(spStream->SetBaseStream(myStream, fmtguid, wfx));
        CheckHr(spVoice->SetOutput(spStream, TRUE));
        logger->LogStepC(L"Client Speak start");
        CheckHr(spVoice->Speak(std::to_wstring(i).c_str(), 0, nullptr));  // speak synchronously
        logger->LogStepC(L"Client Speak end");
    }

    OutputLogs();
}

static void Init()
{
    CheckHr(CoInitialize(nullptr));

    std::locale loc("");
    std::wcout.imbue(loc);
    std::wcin.imbue(loc);
    std::wcerr.imbue(loc);

    CheckHr(spVoice.CoCreateInstance(CLSID_SpVoice));
    CheckHr(logger.CoCreateInstance(__uuidof(Logger)));

    const auto events = SPFEI(SPEI_START_INPUT_STREAM) | SPFEI(SPEI_END_INPUT_STREAM);
    CheckHr(spVoice->SetInterest(events, events));
    CheckHr(spVoice->SetNotifySink(notifySink));

    SelectVoice();
}

static void MainLoop()
{
    std::wcout
        << L"TTS Latency Test Client\n\n"
        << L"1. Select another voice\n"
        << L"2. Store timestamp logs to CSV files (currently " << std::boolalpha << outputCSVFile << L")\n"
        << L"3. Test: Speak synchronously to default audio device\n"
        << L"4. Test: Speak synchronously to custom memory stream\n"
        << L"0. Quit\n\n"
        << L"Enter a number: ";
    UINT i;
    std::wcin >> i;
    switch (i)
    {
    case 0:
        exit(0);
        break;
    case 1:
        SelectVoice();
        break;
    case 2:
        outputCSVFile = !outputCSVFile;
        break;
    case 3:
        TestSyncSpeak();
        break;
    case 4:
        TestSyncSpeakStream();
        break;
    }
}

int main()
{
    try
    {
        Init();
        for (;;)
        {
            try
            {
                MainLoop();
            }
            catch (const std::system_error& ex)
            {
                std::cerr << "Error " << "0x" << std::hex << (UINT)ex.code().value() << ": " << ex.what() << '\n';
            }
            catch (const std::exception& ex)
            {
                std::cerr << "Error: " << ex.what() << '\n';
            }
        }
    }
    catch (const std::system_error& ex)
    {
        std::cerr << "Error " << "0x" << std::hex << (UINT)ex.code().value() << ": " << ex.what() << '\n';
        system("pause");
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << '\n';
        system("pause");
    }
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

// TTSSite.cpp: CTTSSite 的实现

#include "pch.h"
#include "TTSSite.h"

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

// Returns the trailing silence wave data length, in bytes
template <typename SampleType, size_t bytesPerSample = sizeof(SampleType)>
size_t GetTrailingSilenceLengthMono(const BYTE* waveData, size_t length)
{
    if (length < bytesPerSample)
        return 0;

    constexpr SampleType silencePoint = GetSilencePoint<SampleType, bytesPerSample>();
    constexpr SampleType threshold = GetThreshold<SampleType, bytesPerSample>();
    constexpr SampleType minValue = silencePoint - threshold, maxValue = silencePoint + threshold;

    // Check each sample in reverse order
    const BYTE* p = waveData + (length - (length % bytesPerSample));
    SampleType smp;
    do
    {
        p -= bytesPerSample;
        if constexpr (bytesPerSample < sizeof(SampleType))
            smp = SampleType();
        memcpy(&smp, p, bytesPerSample);
        // this sample is out of range, so the trailing silence starts at the next sample
        if (smp < minValue || smp > maxValue)
            return length - (p - waveData) - bytesPerSample;
    } while (p != waveData);

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

static size_t GetTrailingSilenceLength(const WAVEFORMATEX* wfx, const BYTE* waveData, size_t length)
{
    size_t len;
    switch (GetFormatTag(wfx))
    {
    case WAVE_FORMAT_PCM:
        switch (wfx->wBitsPerSample)
        {
        case 8:  // 8-bits are unsigned, others are signed
            len = GetTrailingSilenceLengthMono<BYTE>(waveData, length);
            break;
        case 16:
            len = GetTrailingSilenceLengthMono<SHORT>(waveData, length);
            break;
        case 24:
            len = GetTrailingSilenceLengthMono<LONG, 3>(waveData, length);
            break;
        case 32:
            len = GetTrailingSilenceLengthMono<LONG>(waveData, length);
            break;
        default:
            return 0;
        }
        break;
    case WAVE_FORMAT_IEEE_FLOAT:
        switch (wfx->wBitsPerSample)
        {
        case 32:
            len = GetTrailingSilenceLengthMono<float>(waveData, length);
            break;
        case 64:
            len = GetTrailingSilenceLengthMono<double>(waveData, length);
            break;
        default:
            return 0;
        }
        break;
    default:
        return 0;
    }

    size_t align = wfx->nBlockAlign;
    len += align - 1;
    len -= len % align;  // round up to block (channel) boundaries
    return len;
}


// CTTSSite

STDMETHODIMP CTTSSite::AddEvents(const SPEVENT* pEventArray, ULONG ulCount)
{
    CSpCoTaskMemPtr<SPEVENT> events;
    RETONFAIL(events.Alloc(ulCount));
    memcpy(events, pEventArray, ulCount * sizeof(SPEVENT));
    for (SPEVENT *pEvt = events, *pEnd = events + ulCount; pEvt != pEnd; pEvt++)
    {
        if (pEvt->ullAudioStreamOffset > m_trimmedPrefixBytes)
            pEvt->ullAudioStreamOffset -= m_trimmedPrefixBytes;
        else
            pEvt->ullAudioStreamOffset = 0;
    }
	return m_pSite->AddEvents(events, ulCount);
}

STDMETHODIMP CTTSSite::GetEventInterest(ULONGLONG* pullEventInterest)
{
	return m_pSite->GetEventInterest(pullEventInterest);
}

DWORD STDMETHODCALLTYPE CTTSSite::GetActions(void)
{
	return m_pSite->GetActions();
}

STDMETHODIMP CTTSSite::Write(const void* pBuff, ULONG cb, ULONG* pcbWritten)
{
    ULONG cbWritten = 0;
    if (!m_leadingSilenceDetected)
    {
        const BYTE* p = static_cast<const BYTE*>(pBuff);
        size_t len = GetLeadingSilenceLength(m_pWfx, p, cb);
        if (len == cb)  // the whole chunk is silence, continue detecting
        {
            if (pcbWritten)
                *pcbWritten = len;
            m_trimmedPrefixBytes += len;
            return S_OK;
        }

        HRESULT hr = m_pSite->Write(p + len, cb - len, &cbWritten);
        if (SUCCEEDED(hr))
        {
            if (cbWritten == cb - len)
                cbWritten = cb;
            if (pcbWritten)
                *pcbWritten = cbWritten;
            m_trimmedPrefixBytes += len;
            m_leadingSilenceDetected = true;  // Stop detecting
            wchar_t buf[128];
            swprintf_s(buf, L"Engine audio data written, %.2lf ms / %llu bytes silence skipped",
                m_trimmedPrefixBytes * 1000.0 / m_pWfx->nAvgBytesPerSec,
                m_trimmedPrefixBytes);
            m_pLogger->LogStepC(buf);
        }
        return hr;
    }

	HRESULT hr = m_pSite->Write(pBuff, cb, &cbWritten);
    if (pcbWritten)
        *pcbWritten = cbWritten;
    return hr;
}

STDMETHODIMP CTTSSite::GetRate(long* pRateAdjust)
{
	return m_pSite->GetRate(pRateAdjust);
}

STDMETHODIMP CTTSSite::GetVolume(USHORT* pusVolume)
{
	return m_pSite->GetVolume(pusVolume);
}

STDMETHODIMP CTTSSite::GetSkipInfo(SPVSKIPTYPE* peType, long* plNumItems)
{
	return m_pSite->GetSkipInfo(peType, plNumItems);
}

STDMETHODIMP CTTSSite::CompleteSkip(long ulNumSkipped)
{
	return m_pSite->CompleteSkip(ulNumSkipped);
}

void CTTSSite::InitSite(ISpTTSEngineSite* pSite, const WAVEFORMATEX* pWaveFormatEx)
{
	m_pSite = pSite;
    m_pWfx = pWaveFormatEx;
}

void CTTSSite::CompleteSpeak()
{
}

HRESULT CTTSSite::Create(ISpTTSEngineSite* pSiteIn, const WAVEFORMATEX* pWaveFormatEx, ITTSSite** ppSiteOut)
{
	CComObject<CTTSSite>* pSite;
	RETONFAIL(CComObject<CTTSSite>::CreateInstance(&pSite));
    pSite->AddRef();
	pSite->InitSite(pSiteIn, pWaveFormatEx);
	*ppSiteOut = pSite;
	return S_OK;
}

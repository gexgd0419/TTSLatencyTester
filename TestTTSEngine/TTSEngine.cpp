// TTSEngine.cpp: CTTSEngine 的实现

#include "pch.h"
#include "TTSEngine.h"
#include "TTSSite.h"


// CTTSEngine

STDMETHODIMP CTTSEngine::SetObjectToken(ISpObjectToken* pToken) noexcept
{
	m_pLogger->LogStepC(L"Engine SetObjectToken start");
	RETONFAIL(SpGenericSetObjectToken(pToken, m_pToken));
	CSpDynamicString engineClsid;
	RETONFAIL(pToken->GetStringValue(L"EngineCLSID", &engineClsid));
	CLSID clsid;
	RETONFAIL(CLSIDFromString(engineClsid, &clsid));
	RETONFAIL(m_pEngine.CoCreateInstance(clsid));
	CComPtr<ISpObjectWithToken> objectWithToken;
	if (SUCCEEDED(m_pEngine.QueryInterface(&objectWithToken)))
	{
		CSpDynamicString tokenId;
		RETONFAIL(pToken->GetStringValue(L"TokenId", &tokenId));
		CComPtr<ISpObjectToken> token;
		RETONFAIL(SpGetTokenFromId(tokenId, &token));
		RETONFAIL(objectWithToken->SetObjectToken(token));
	}
	m_pLogger->LogStepC(L"Engine SetObjectToken end");
	return S_OK;
}

STDMETHODIMP CTTSEngine::Speak(DWORD dwSpeakFlags, REFGUID rguidFormatId, const WAVEFORMATEX* pWaveFormatEx, const SPVTEXTFRAG* pTextFragList, ISpTTSEngineSite* pOutputSite) noexcept
{
	if (!m_pEngine)
		return SPERR_UNINITIALIZED;
	m_pLogger->LogStepC(L"Engine Speak start");
	CComPtr<ITTSSite> pMySite;
	RETONFAIL(CTTSSite::Create(pOutputSite, pWaveFormatEx, &pMySite));
	HRESULT hr = m_pEngine->Speak(dwSpeakFlags, rguidFormatId, pWaveFormatEx, pTextFragList, pMySite);
	pMySite->CompleteSpeak();
	m_pLogger->LogStepC(L"Engine Speak end");
	return hr;
}

static std::wstring GetFormatDescription(const WAVEFORMATEX* wfx)
{
	if (!wfx)
		return L"(null)";
	std::wstringstream desc;
	switch (wfx->wFormatTag)
	{
	case WAVE_FORMAT_PCM:
		desc << L"PCM ";
		break;
	case WAVE_FORMAT_IEEE_FLOAT:
		desc << L"PCM float ";
		break;
	default:
		desc << L"Unknown wave format (" << wfx->wFormatTag << L") ";
		break;
	}
	desc << (wfx->nSamplesPerSec / 1000.0) << L" kHz ";
	desc << wfx->wBitsPerSample << L" bits ";
	switch (wfx->nChannels)
	{
	case 1:
		desc << L"Mono";
		break;
	case 2:
		desc << L"Stereo";
		break;
	default:
		desc << wfx->nChannels << L" channels";
		break;
	}
	return desc.str();
}

STDMETHODIMP CTTSEngine::GetOutputFormat(const GUID* pTargetFormatId, const WAVEFORMATEX* pTargetWaveFormatEx, GUID* pDesiredFormatId, WAVEFORMATEX** ppCoMemDesiredWaveFormatEx) noexcept
{
	if (!m_pEngine)
		return SPERR_UNINITIALIZED;
	HRESULT hr = m_pEngine->GetOutputFormat(pTargetFormatId, pTargetWaveFormatEx, pDesiredFormatId, ppCoMemDesiredWaveFormatEx);
	RETONFAIL(hr);
	std::wstring log = L"Engine output format set, target: ";
	log += GetFormatDescription(pTargetWaveFormatEx);
	log += L", final: ";
	log += GetFormatDescription(*ppCoMemDesiredWaveFormatEx);
	m_pLogger->LogStepC(log.c_str());
	return hr;
}

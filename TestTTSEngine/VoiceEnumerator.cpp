// VoiceEnumerator.cpp: CVoiceEnumerator 的实现

#include "pch.h"
#include "VoiceEnumerator.h"
#include "VoiceKey.h"


// CVoiceEnumerator

HRESULT CVoiceEnumerator::FinalConstruct()
{
	CComPtr<ISpObjectTokenEnumBuilder> pBuilder;
	RETONFAIL(pBuilder.CoCreateInstance(CLSID_SpObjectTokenEnum));
	RETONFAIL(pBuilder->SetAttribs(nullptr, nullptr));

	static bool isEnumerating = false;  // prevent recursive calls
	if (isEnumerating)
	{
		// return empty list
		RETONFAIL(pBuilder.QueryInterface(&m_pEnum));
		return S_OK;
	}

	CComPtr<IEnumSpObjectTokens> pEnum;
	isEnumerating = true;
	HRESULT hr = SpEnumTokens(SPCAT_VOICES, nullptr, nullptr, &pEnum);
	isEnumerating = false;
	RETONFAIL(hr);

	for (CComPtr<ISpObjectToken> pToken; pEnum->Next(1, &pToken, nullptr) == S_OK; pToken.Release())
	{
		CSpDynamicString idOld, idNew;
		RETONFAIL(pToken->GetId(&idOld));
		PWCHAR pNamePart = wcsrchr(idOld, L'\\');
		if (pNamePart)
			pNamePart++;
		else
			pNamePart = idOld;
		RETONFAIL(idNew.Append2HR(SPCAT_VOICES L"\\TokenEnums\\TestTTSEngineEnumerator\\", pNamePart));

		CSpDynamicString clsid;
		RETONFAIL(pToken->GetStringValue(L"CLSID", &clsid));

		CComPtr<ISpDataKey> pMyKey;
		RETONFAIL(CVoiceKey::Create(pToken, std::move(clsid), std::move(idOld), &pMyKey));

		CComPtr<ISpObjectToken> pProxy;
		RETONFAIL(pProxy.CoCreateInstance(CLSID_SpObjectToken));
		RETONFAIL(CComQIPtr<ISpObjectTokenInit>(pProxy)->InitFromDataKey(SPCAT_VOICES, idNew, pMyKey));
		RETONFAIL(pBuilder->AddTokens(1, &pProxy.p));
	}

	RETONFAIL(pBuilder.QueryInterface(&m_pEnum));
	return S_OK;
}

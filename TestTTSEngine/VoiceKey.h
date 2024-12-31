﻿// VoiceKey.h: CVoiceKey 的声明

#pragma once
#include "resource.h"       // 主符号



#include "TestTTSEngine_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CVoiceKey

class ATL_NO_VTABLE CVoiceKey :
	public CComObjectRootEx<CComMultiThreadModel>,
    public ISpDataKey
{
public:
	CVoiceKey()
	{
	}

DECLARE_NOT_AGGREGATABLE(CVoiceKey)

BEGIN_COM_MAP(CVoiceKey)
	COM_INTERFACE_ENTRY(ISpDataKey)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
    STDMETHODIMP SetData(LPCWSTR pszValueName, ULONG cbData, const BYTE* pData);
    STDMETHODIMP GetData(LPCWSTR pszValueName, ULONG* pcbData, BYTE* pData);
    STDMETHODIMP SetStringValue(LPCWSTR pszValueName, LPCWSTR pszValue);
    STDMETHODIMP GetStringValue(LPCWSTR pszValueName, LPWSTR* ppszValue);
    STDMETHODIMP SetDWORD(LPCWSTR pszValueName, DWORD dwValue);
    STDMETHODIMP GetDWORD(LPCWSTR pszValueName, DWORD* pdwValue);
    STDMETHODIMP OpenKey(LPCWSTR pszSubKeyName, ISpDataKey** ppSubKey);
    STDMETHODIMP CreateKey(LPCWSTR pszSubKey, ISpDataKey** ppSubKey);
    STDMETHODIMP DeleteKey(LPCWSTR pszSubKey);
    STDMETHODIMP DeleteValue(LPCWSTR pszValueName);
    STDMETHODIMP EnumKeys(ULONG Index, LPWSTR* ppszSubKeyName);
    STDMETHODIMP EnumValues(ULONG Index, LPWSTR* ppszValueName);

    void InitKey(ISpDataKey* pKey, CSpDynamicString&& pEngineClsid, CSpDynamicString&& pTokenId);

    static HRESULT Create(ISpDataKey* pKeyIn, CSpDynamicString&& pEngineClsid, CSpDynamicString&& pTokenId, ISpDataKey** ppKeyOut);
    static HRESULT Create(ISpDataKey* pKeyIn, const CSpDynamicString& pEngineClsid, const CSpDynamicString& pTokenId, ISpDataKey** ppKeyOut);

private:
    CComPtr<ISpDataKey> m_pKey;
    CSpDynamicString m_pEngineClsid, m_pTokenId;
};

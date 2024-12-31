// VoiceKey.cpp: CVoiceKey 的实现

#include "pch.h"
#include "VoiceKey.h"


// CVoiceKey

static bool IsHexString(PCWSTR str)
{
    for (; *str; str++)
    {
        WCHAR ch = *str;
        if (ch >= '0' && ch <= '9'
            || ch >= 'A' && ch <= 'F'
            || ch >= 'a' && ch <= 'f')
        {
            continue;
        }
        else
            return false;
    }
    return true;
}

STDMETHODIMP CVoiceKey::SetData(LPCWSTR pszValueName, ULONG cbData, const BYTE* pData)
{
    return m_pKey->SetData(pszValueName, cbData, pData);
}

STDMETHODIMP CVoiceKey::GetData(LPCWSTR pszValueName, ULONG* pcbData, BYTE* pData)
{
    return m_pKey->GetData(pszValueName, pcbData, pData);
}

STDMETHODIMP CVoiceKey::SetStringValue(LPCWSTR pszValueName, LPCWSTR pszValue)
{
    return m_pKey->SetStringValue(pszValueName, pszValue);
}

STDMETHODIMP CVoiceKey::GetStringValue(LPCWSTR pszValueName, LPWSTR* ppszValue)
{
    if (!ppszValue)
        return E_POINTER;
    if (!pszValueName || pszValueName[0] == L'\0' || IsHexString(pszValueName)
        || _wcsicmp(pszValueName, L"Name") == 0)
    {
        CSpDynamicString oldname, newname;
        RETONFAIL(m_pKey->GetStringValue(pszValueName, &oldname));
        RETONFAIL(newname.Append2HR(L"[Forwarded] ", oldname));
        *ppszValue = newname.Detach();
        return S_OK;
    }
    else if (_wcsicmp(pszValueName, L"CLSID") == 0)
    {
        CSpDynamicString clsid = L"{66e60648-139d-4916-96cb-a492ea4513d9}";
        *ppszValue = clsid.Copy();
        return *ppszValue ? S_OK : E_OUTOFMEMORY;
    }
    else if (_wcsicmp(pszValueName, L"EngineCLSID") == 0)
    {
        *ppszValue = m_pEngineClsid.Copy();
        return *ppszValue ? S_OK : E_OUTOFMEMORY;
    }
    else if (_wcsicmp(pszValueName, L"TokenId") == 0)
    {
        *ppszValue = m_pTokenId.Copy();
        return *ppszValue ? S_OK : E_OUTOFMEMORY;
    }
    return m_pKey->GetStringValue(pszValueName, ppszValue);
}

STDMETHODIMP CVoiceKey::SetDWORD(LPCWSTR pszValueName, DWORD dwValue)
{
    return m_pKey->SetDWORD(pszValueName, dwValue);
}

STDMETHODIMP CVoiceKey::GetDWORD(LPCWSTR pszValueName, DWORD* pdwValue)
{
    return m_pKey->GetDWORD(pszValueName, pdwValue);
}

STDMETHODIMP CVoiceKey::OpenKey(LPCWSTR pszSubKeyName, ISpDataKey** ppSubKey)
{
    if (!ppSubKey)
        return E_POINTER;
    CComPtr<ISpDataKey> pOrigKey;
    RETONFAIL(m_pKey->OpenKey(pszSubKeyName, &pOrigKey));
    CComPtr<ISpDataKey> pMyKey;
    RETONFAIL(Create(pOrigKey, m_pEngineClsid, m_pTokenId, &pMyKey));
    *ppSubKey = pMyKey.Detach();
    return S_OK;
}

STDMETHODIMP CVoiceKey::CreateKey(LPCWSTR pszSubKey, ISpDataKey** ppSubKey)
{
    HRESULT hr = OpenKey(pszSubKey, ppSubKey);
    if (hr == SPERR_NOT_FOUND)
        return m_pKey->CreateKey(pszSubKey, ppSubKey);
    return hr;
}

STDMETHODIMP CVoiceKey::DeleteKey(LPCWSTR pszSubKey)
{
    return m_pKey->DeleteKey(pszSubKey);
}

STDMETHODIMP CVoiceKey::DeleteValue(LPCWSTR pszValueName)
{
    return m_pKey->DeleteKey(pszValueName);
}

STDMETHODIMP CVoiceKey::EnumKeys(ULONG Index, LPWSTR* ppszSubKeyName)
{
    return m_pKey->EnumKeys(Index, ppszSubKeyName);
}

STDMETHODIMP CVoiceKey::EnumValues(ULONG Index, LPWSTR* ppszValueName)
{
    if (!ppszValueName)
        return E_POINTER;
    CSpDynamicString ret;
    switch (Index)
    {
    case 0:
        ret = L"EngineCLSID";
        break;
    case 1:
        ret = L"TokenId";
        break;
    case 2:
        ret = "Name";
        break;
    default:
        Index -= 3;
        return m_pKey->EnumKeys(Index, ppszValueName);
    }
    *ppszValueName = ret.Detach();
    return *ppszValueName ? S_OK : E_OUTOFMEMORY;
}

void CVoiceKey::InitKey(ISpDataKey* pKey, CSpDynamicString&& pEngineClsid, CSpDynamicString&& pTokenId)
{
    m_pKey = pKey;
    m_pEngineClsid.Attach(pEngineClsid.Detach());
    m_pTokenId.Attach(pTokenId.Detach());
}

HRESULT CVoiceKey::Create(ISpDataKey* pKeyIn, CSpDynamicString&& pEngineClsid, CSpDynamicString&& pTokenId, ISpDataKey** ppKeyOut)
{
    CComObject<CVoiceKey>* pKey;
    RETONFAIL(CComObject<CVoiceKey>::CreateInstance(&pKey));
    pKey->AddRef();
    pKey->InitKey(pKeyIn, std::move(pEngineClsid), std::move(pTokenId));
    *ppKeyOut = pKey;
    return S_OK;
}

HRESULT CVoiceKey::Create(ISpDataKey* pKeyIn, const CSpDynamicString& pEngineClsid, const CSpDynamicString& pTokenId, ISpDataKey** ppKeyOut)
{
    CSpDynamicString clsid(pEngineClsid);
    if (!clsid) return E_OUTOFMEMORY;
    CSpDynamicString tokenid(pTokenId);
    if (!tokenid) return E_OUTOFMEMORY;
    return Create(pKeyIn, std::move(clsid), std::move(tokenid), ppKeyOut);
}

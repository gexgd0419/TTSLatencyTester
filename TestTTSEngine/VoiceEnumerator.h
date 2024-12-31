// VoiceEnumerator.h: CVoiceEnumerator 的声明

#pragma once
#include "resource.h"       // 主符号



#include "TestTTSEngine_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CVoiceEnumerator

class ATL_NO_VTABLE CVoiceEnumerator :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVoiceEnumerator, &CLSID_VoiceEnumerator>,
	public IEnumSpObjectTokens
{
public:
	CVoiceEnumerator()
	{
	}

DECLARE_REGISTRY_RESOURCEID(107)

DECLARE_NOT_AGGREGATABLE(CVoiceEnumerator)

BEGIN_COM_MAP(CVoiceEnumerator)
	COM_INTERFACE_ENTRY(IEnumSpObjectTokens)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();

	void FinalRelease()
	{
	}

protected:
	CComPtr<IEnumSpObjectTokens> m_pEnum;

public:
	// manually forward calls to inner enumerator object
	STDMETHODIMP Next(ULONG celt, ISpObjectToken** pelt, ULONG* pceltFetched) noexcept override
	{
		return m_pEnum->Next(celt, pelt, pceltFetched);
	}
	STDMETHODIMP Skip(ULONG celt) noexcept override
	{
		return m_pEnum->Skip(celt);
	}
	STDMETHODIMP Reset(void) noexcept override
	{
		return m_pEnum->Reset();
	}
	STDMETHODIMP Clone(IEnumSpObjectTokens** ppEnum) noexcept override
	{
		return m_pEnum->Clone(ppEnum);
	}
	STDMETHODIMP Item(ULONG Index, ISpObjectToken** ppToken) noexcept override
	{
		return m_pEnum->Item(Index, ppToken);
	}
	STDMETHODIMP GetCount(ULONG* pCount) noexcept override
	{
		return m_pEnum->GetCount(pCount);
	}
};

OBJECT_ENTRY_AUTO(__uuidof(VoiceEnumerator), CVoiceEnumerator)

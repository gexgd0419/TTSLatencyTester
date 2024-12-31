// TTSEngine.h: CTTSEngine 的声明

#pragma once
#include "resource.h"       // 主符号

#include "Logger.h"

#include "TestTTSEngine_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CTTSEngine

class ATL_NO_VTABLE CTTSEngine :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CTTSEngine, &CLSID_TTSEngine>,
	public ISpTTSEngine,
	public ISpObjectWithToken
{
public:
	CTTSEngine()
	{
	}

DECLARE_REGISTRY_RESOURCEID(106)

DECLARE_NOT_AGGREGATABLE(CTTSEngine)

BEGIN_COM_MAP(CTTSEngine)
	COM_INTERFACE_ENTRY(ISpTTSEngine)
	COM_INTERFACE_ENTRY(ISpObjectWithToken)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return CLogger::CreateInstance(&m_pLogger);
	}

	void FinalRelease()
	{
	}

	// ISpObjectWithToken 
	STDMETHODIMP SetObjectToken(ISpObjectToken* pToken) noexcept override;
	STDMETHODIMP GetObjectToken(ISpObjectToken** ppToken) noexcept override
	{
		return SpGenericGetObjectToken(ppToken, m_pToken);
	}

	// ISpTTSEngine
	STDMETHODIMP Speak(DWORD dwSpeakFlags,
		REFGUID rguidFormatId, const WAVEFORMATEX* pWaveFormatEx,
		const SPVTEXTFRAG* pTextFragList, ISpTTSEngineSite* pOutputSite) noexcept override;
	STDMETHODIMP GetOutputFormat(const GUID* pTargetFormatId, const WAVEFORMATEX* pTargetWaveFormatEx,
		GUID* pDesiredFormatId, WAVEFORMATEX** ppCoMemDesiredWaveFormatEx) noexcept override;

private:
	CComPtr<ISpObjectToken> m_pToken;
	CComPtr<ISpTTSEngine> m_pEngine;  // forward target
	CComPtr<ILogger> m_pLogger;
};

OBJECT_ENTRY_AUTO(__uuidof(TTSEngine), CTTSEngine)

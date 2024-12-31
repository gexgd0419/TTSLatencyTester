// TTSSite.h: CTTSSite 的声明

#pragma once
#include "resource.h"       // 主符号

#include "Logger.h"

#include "TestTTSEngine_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;

struct ATL_NO_VTABLE ITTSSite : public ISpTTSEngineSite
{
	virtual void CompleteSpeak() = 0;
};

// CTTSSite

class ATL_NO_VTABLE CTTSSite :
	public CComObjectRootEx<CComMultiThreadModel>,
	public ITTSSite
{
public:
	CTTSSite()
	{
	}

DECLARE_NO_REGISTRY()

DECLARE_NOT_AGGREGATABLE(CTTSSite)

BEGIN_COM_MAP(CTTSSite)
	COM_INTERFACE_ENTRY(ISpTTSEngineSite)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return CLogger::CreateInstance(&m_pLogger);
	}

	void FinalRelease()
	{
	}

public:
	STDMETHODIMP AddEvents(const SPEVENT* pEventArray, ULONG ulCount);
	STDMETHODIMP GetEventInterest(ULONGLONG* pullEventInterest);

	DWORD STDMETHODCALLTYPE GetActions(void);
	STDMETHODIMP Write(const void* pBuff, ULONG cb, ULONG* pcbWritten);
	STDMETHODIMP GetRate(long* pRateAdjust);
	STDMETHODIMP GetVolume(USHORT* pusVolume);
	STDMETHODIMP GetSkipInfo(SPVSKIPTYPE* peType, long* plNumItems);
	STDMETHODIMP CompleteSkip(long ulNumSkipped);

	void InitSite(ISpTTSEngineSite* pSite, const WAVEFORMATEX* pWaveFormatEx);
	void CompleteSpeak();

	static HRESULT Create(ISpTTSEngineSite* pSiteIn, const WAVEFORMATEX* pWaveFormatEx, ITTSSite** ppSiteOut);

private:
	CComPtr<ISpTTSEngineSite> m_pSite;
	CComPtr<ILogger> m_pLogger;
	const WAVEFORMATEX* m_pWfx;
	ULONGLONG m_trimmedPrefixBytes = 0;
	bool m_leadingSilenceDetected = false;
};

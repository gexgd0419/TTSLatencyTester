// Logger.h: CLogger 的声明

#pragma once
#include "resource.h"       // 主符号

#include "LogItems.h"

#include "TestTTSEngine_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CLogger

class ATL_NO_VTABLE CLogger :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CLogger, &CLSID_Logger>,
	public IDispatchImpl<ILogger, &IID_ILogger, &LIBID_TestTTSEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CLogger()
	{
	}

DECLARE_REGISTRY_RESOURCEID(110)

DECLARE_NOT_AGGREGATABLE(CLogger)

BEGIN_COM_MAP(CLogger)
	COM_INTERFACE_ENTRY(ILogger)
	COM_INTERFACE_ENTRY(IDispatch)
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
	STDMETHOD(LogStart)();
	STDMETHOD(LogStep)(BSTR logMessage);
	STDMETHOD(LogStepC)(LPCWSTR logMessage);
	STDMETHOD(LogEnd)();
	STDMETHOD(GetLogItems)(ILogItems** ppItems);

private:
	static std::recursive_mutex s_mutex;
	static bool s_started;
	static LONGLONG s_startTime;
	static std::vector<LogItemData> s_logs;
	static void Init();
};

OBJECT_ENTRY_AUTO(__uuidof(Logger), CLogger)

// LogItem.h: CLogItem 的声明

#pragma once
#include "resource.h"       // 主符号



#include "TestTTSEngine_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;

struct LogItemData
{
	LONGLONG time;
	CComBSTR message;
	LogItemData(LONGLONG time, CComBSTR message) : time(time), message(std::move(message)) {}
};


// CLogItem

class ATL_NO_VTABLE CLogItem :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CLogItem, &CLSID_LogItem>,
	public IDispatchImpl<ILogItem, &IID_ILogItem, &LIBID_TestTTSEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CLogItem()
	{
	}

DECLARE_NO_REGISTRY()

DECLARE_NOT_AGGREGATABLE(CLogItem)

BEGIN_COM_MAP(CLogItem)
	COM_INTERFACE_ENTRY(ILogItem)
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
	STDMETHOD(get_ElapsedSeconds)(DOUBLE* pVal);
	STDMETHOD(get_Message)(BSTR* pVal);

	void SetData(const LogItemData* pData)
	{
		m_pData = pData;
	}

private:
	const LogItemData* m_pData = nullptr;
};

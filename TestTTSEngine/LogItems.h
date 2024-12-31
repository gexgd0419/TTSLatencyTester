// LogItems.h: CLogItems 的声明

#pragma once
#include "resource.h"       // 主符号

#include "LogItem.h"

#include "TestTTSEngine_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;

template <class DestinationType, class SourceType>
class ConversionCopy
{
public:
	static void init(DestinationType* p)
	{
		_Copy<DestinationType>::init(p);
	}
	static void destroy(DestinationType* p)
	{
		_Copy<DestinationType>::destroy(p);
	}
	static HRESULT copy(DestinationType* pTo, const SourceType* pFrom)
	{
		*pTo = DestinationType(*pFrom);
		return S_OK;
	}
};

template <class SourceType>
class CopyToVariant
{
public:
	static void init(VARIANT* p)
	{
		_Copy<VARIANT>::init(p);
	}
	static void destroy(VARIANT* p)
	{
		_Copy<VARIANT>::destroy(p);
	}
	static HRESULT copy(VARIANT* pTo, const SourceType* pFrom)
	{
		return CComVariant(*pFrom).Detach(pTo);
	}
};

using LogItemsContainerType = std::vector<CComPtr<ILogItem>>;
using LogItemsEnumeratorType = CComEnumOnSTL<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, CopyToVariant<CComPtr<ILogItem>>, LogItemsContainerType>;
using LogItemsCollectionType = ICollectionOnSTLImpl<ILogItems, LogItemsContainerType, ILogItem*, ConversionCopy<ILogItem*, CComPtr<ILogItem>>, LogItemsEnumeratorType>;

// CLogItems

class ATL_NO_VTABLE CLogItems :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CLogItems, &CLSID_LogItems>,
	public IDispatchImpl<LogItemsCollectionType, &IID_ILogItems, &LIBID_TestTTSEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CLogItems()
	{
	}

DECLARE_NO_REGISTRY()

DECLARE_NOT_AGGREGATABLE(CLogItems)

BEGIN_COM_MAP(CLogItems)
	COM_INTERFACE_ENTRY(ILogItems)
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
	HRESULT InitCollection(const std::vector<LogItemData>& source);
};

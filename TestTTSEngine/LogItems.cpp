// LogItems.cpp: CLogItems 的实现

#include "pch.h"
#include "LogItems.h"


// CLogItems

HRESULT CLogItems::InitCollection(const std::vector<LogItemData>& source)
{
	for (const auto& item : source)
	{
		CComObject<CLogItem>* pItem;
		RETONFAIL(CComObject<CLogItem>::CreateInstance(&pItem));
		pItem->AddRef();
		pItem->SetData(&item);
		m_coll.push_back(CComQIPtr<ILogItem>(pItem));
	}
	return S_OK;
}

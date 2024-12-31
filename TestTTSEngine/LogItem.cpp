// LogItem.cpp: CLogItem 的实现

#include "pch.h"
#include "LogItem.h"


// CLogItem



STDMETHODIMP CLogItem::get_ElapsedSeconds(DOUBLE* pVal)
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    *pVal = static_cast<DOUBLE>(m_pData->time) / freq.QuadPart;
    return S_OK;
}


STDMETHODIMP CLogItem::get_Message(BSTR* pVal)
{
    *pVal = m_pData->message.Copy();
    return *pVal ? S_OK : E_OUTOFMEMORY;
}

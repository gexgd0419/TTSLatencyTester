// Logger.cpp: CLogger 的实现

#include "pch.h"
#include "Logger.h"



// CLogger

std::recursive_mutex CLogger::s_mutex;
bool CLogger::s_started = false;
LONGLONG CLogger::s_startTime = 0;
std::vector<LogItemData> CLogger::s_logs;

STDMETHODIMP CLogger::LogStart()
{
    std::lock_guard lock(s_mutex);
    s_started = false;
    Init();
    return S_OK;
}


STDMETHODIMP CLogger::LogStep(BSTR logMessage)
{
    std::lock_guard lock(s_mutex);
    Init();
    LARGE_INTEGER curtime;
    QueryPerformanceCounter(&curtime);
    CComBSTR msg;
    msg.Attach(logMessage);
    s_logs.emplace_back(curtime.QuadPart - s_startTime, msg);
    msg.Detach();
    return S_OK;
}


STDMETHODIMP CLogger::LogStepC(LPCWSTR logMessage)
{
    std::lock_guard lock(s_mutex);
    Init();
    LARGE_INTEGER curtime;
    QueryPerformanceCounter(&curtime);
    s_logs.emplace_back(curtime.QuadPart - s_startTime, logMessage);
    return S_OK;
}


STDMETHODIMP CLogger::LogEnd()
{
    s_started = false;
    return S_OK;
}


STDMETHODIMP CLogger::GetLogItems(ILogItems** ppItems)
{
    CComObject<CLogItems>* pItems;
    RETONFAIL(CComObject<CLogItems>::CreateInstance(&pItems));
    pItems->AddRef();
    HRESULT hr = pItems->InitCollection(s_logs);
    if (FAILED(hr))
    {
        pItems->Release();
        return hr;
    }
    *ppItems = pItems;
    return S_OK;
}


void CLogger::Init()
{
    if (s_started)
        return;
    s_logs.clear();
    s_logs.reserve(32);
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    s_startTime = time.QuadPart;
    s_started = true;
}

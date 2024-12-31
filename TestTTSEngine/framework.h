#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的


#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <queue>

#define RETONFAIL(hresult) if (HRESULT __hr = hresult; FAILED(__hr)) [[unlikely]] return __hr

using ATL::CComPtr; // put this here to avoid 'CComPtr undeclared' in spddkhlp.h

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning(suppress : 4996) // 'GetVersionExW' deprecated
#include <spddkhlp.h>
#include <ShObjIdl.h>

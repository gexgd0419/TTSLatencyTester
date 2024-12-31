

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for TestTTSEngine.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __TestTTSEngine_i_h__
#define __TestTTSEngine_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __ILogItem_FWD_DEFINED__
#define __ILogItem_FWD_DEFINED__
typedef interface ILogItem ILogItem;

#endif 	/* __ILogItem_FWD_DEFINED__ */


#ifndef __ILogItems_FWD_DEFINED__
#define __ILogItems_FWD_DEFINED__
typedef interface ILogItems ILogItems;

#endif 	/* __ILogItems_FWD_DEFINED__ */


#ifndef __ILogger_FWD_DEFINED__
#define __ILogger_FWD_DEFINED__
typedef interface ILogger ILogger;

#endif 	/* __ILogger_FWD_DEFINED__ */


#ifndef __TTSEngine_FWD_DEFINED__
#define __TTSEngine_FWD_DEFINED__

#ifdef __cplusplus
typedef class TTSEngine TTSEngine;
#else
typedef struct TTSEngine TTSEngine;
#endif /* __cplusplus */

#endif 	/* __TTSEngine_FWD_DEFINED__ */


#ifndef __VoiceEnumerator_FWD_DEFINED__
#define __VoiceEnumerator_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoiceEnumerator VoiceEnumerator;
#else
typedef struct VoiceEnumerator VoiceEnumerator;
#endif /* __cplusplus */

#endif 	/* __VoiceEnumerator_FWD_DEFINED__ */


#ifndef __Logger_FWD_DEFINED__
#define __Logger_FWD_DEFINED__

#ifdef __cplusplus
typedef class Logger Logger;
#else
typedef struct Logger Logger;
#endif /* __cplusplus */

#endif 	/* __Logger_FWD_DEFINED__ */


#ifndef __LogItem_FWD_DEFINED__
#define __LogItem_FWD_DEFINED__

#ifdef __cplusplus
typedef class LogItem LogItem;
#else
typedef struct LogItem LogItem;
#endif /* __cplusplus */

#endif 	/* __LogItem_FWD_DEFINED__ */


#ifndef __LogItems_FWD_DEFINED__
#define __LogItems_FWD_DEFINED__

#ifdef __cplusplus
typedef class LogItems LogItems;
#else
typedef struct LogItems LogItems;
#endif /* __cplusplus */

#endif 	/* __LogItems_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "sapiddk.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ILogItem_INTERFACE_DEFINED__
#define __ILogItem_INTERFACE_DEFINED__

/* interface ILogItem */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ILogItem;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("10f878b1-d659-4e54-b4ea-dae07d2cffba")
    ILogItem : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ElapsedSeconds( 
            /* [retval][out] */ DOUBLE *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Message( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ILogItemVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILogItem * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILogItem * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILogItem * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILogItem * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILogItem * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILogItem * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILogItem * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ILogItem, get_ElapsedSeconds)
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ElapsedSeconds )( 
            ILogItem * This,
            /* [retval][out] */ DOUBLE *pVal);
        
        DECLSPEC_XFGVIRT(ILogItem, get_Message)
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Message )( 
            ILogItem * This,
            /* [retval][out] */ BSTR *pVal);
        
        END_INTERFACE
    } ILogItemVtbl;

    interface ILogItem
    {
        CONST_VTBL struct ILogItemVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILogItem_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILogItem_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILogItem_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILogItem_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ILogItem_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ILogItem_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ILogItem_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ILogItem_get_ElapsedSeconds(This,pVal)	\
    ( (This)->lpVtbl -> get_ElapsedSeconds(This,pVal) ) 

#define ILogItem_get_Message(This,pVal)	\
    ( (This)->lpVtbl -> get_Message(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILogItem_INTERFACE_DEFINED__ */


#ifndef __ILogItems_INTERFACE_DEFINED__
#define __ILogItems_INTERFACE_DEFINED__

/* interface ILogItems */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ILogItems;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6700fdde-8ace-4a61-99ed-2a32738b43cd")
    ILogItems : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppUnk) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long Index,
            /* [retval][out] */ ILogItem **ppItem) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pCount) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ILogItemsVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILogItems * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILogItems * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILogItems * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILogItems * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILogItems * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILogItems * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILogItems * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ILogItems, get__NewEnum)
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            ILogItems * This,
            /* [retval][out] */ IUnknown **ppUnk);
        
        DECLSPEC_XFGVIRT(ILogItems, get_Item)
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            ILogItems * This,
            /* [in] */ long Index,
            /* [retval][out] */ ILogItem **ppItem);
        
        DECLSPEC_XFGVIRT(ILogItems, get_Count)
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            ILogItems * This,
            /* [retval][out] */ long *pCount);
        
        END_INTERFACE
    } ILogItemsVtbl;

    interface ILogItems
    {
        CONST_VTBL struct ILogItemsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILogItems_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILogItems_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILogItems_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILogItems_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ILogItems_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ILogItems_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ILogItems_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ILogItems_get__NewEnum(This,ppUnk)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppUnk) ) 

#define ILogItems_get_Item(This,Index,ppItem)	\
    ( (This)->lpVtbl -> get_Item(This,Index,ppItem) ) 

#define ILogItems_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILogItems_INTERFACE_DEFINED__ */


#ifndef __ILogger_INTERFACE_DEFINED__
#define __ILogger_INTERFACE_DEFINED__

/* interface ILogger */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ILogger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("f6859cd7-f2bc-4597-8981-e56c1266db62")
    ILogger : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE LogStart( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE LogStep( 
            /* [in] */ BSTR logMessage) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE LogStepC( 
            /* [in] */ LPCWSTR logMessage) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE LogEnd( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetLogLines( 
            /* [retval][out] */ BSTR *outLines) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetLogItems( 
            /* [retval][out] */ ILogItems **ppItems) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ILoggerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILogger * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILogger * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILogger * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILogger * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILogger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILogger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILogger * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ILogger, LogStart)
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LogStart )( 
            ILogger * This);
        
        DECLSPEC_XFGVIRT(ILogger, LogStep)
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LogStep )( 
            ILogger * This,
            /* [in] */ BSTR logMessage);
        
        DECLSPEC_XFGVIRT(ILogger, LogStepC)
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *LogStepC )( 
            ILogger * This,
            /* [in] */ LPCWSTR logMessage);
        
        DECLSPEC_XFGVIRT(ILogger, LogEnd)
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LogEnd )( 
            ILogger * This);
        
        DECLSPEC_XFGVIRT(ILogger, GetLogLines)
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetLogLines )( 
            ILogger * This,
            /* [retval][out] */ BSTR *outLines);
        
        DECLSPEC_XFGVIRT(ILogger, GetLogItems)
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetLogItems )( 
            ILogger * This,
            /* [retval][out] */ ILogItems **ppItems);
        
        END_INTERFACE
    } ILoggerVtbl;

    interface ILogger
    {
        CONST_VTBL struct ILoggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILogger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILogger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILogger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILogger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ILogger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ILogger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ILogger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ILogger_LogStart(This)	\
    ( (This)->lpVtbl -> LogStart(This) ) 

#define ILogger_LogStep(This,logMessage)	\
    ( (This)->lpVtbl -> LogStep(This,logMessage) ) 

#define ILogger_LogStepC(This,logMessage)	\
    ( (This)->lpVtbl -> LogStepC(This,logMessage) ) 

#define ILogger_LogEnd(This)	\
    ( (This)->lpVtbl -> LogEnd(This) ) 

#define ILogger_GetLogLines(This,outLines)	\
    ( (This)->lpVtbl -> GetLogLines(This,outLines) ) 

#define ILogger_GetLogItems(This,ppItems)	\
    ( (This)->lpVtbl -> GetLogItems(This,ppItems) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILogger_INTERFACE_DEFINED__ */



#ifndef __TestTTSEngineLib_LIBRARY_DEFINED__
#define __TestTTSEngineLib_LIBRARY_DEFINED__

/* library TestTTSEngineLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_TestTTSEngineLib;

EXTERN_C const CLSID CLSID_TTSEngine;

#ifdef __cplusplus

class DECLSPEC_UUID("66e60648-139d-4916-96cb-a492ea4513d9")
TTSEngine;
#endif

EXTERN_C const CLSID CLSID_VoiceEnumerator;

#ifdef __cplusplus

class DECLSPEC_UUID("478bf4c5-4918-49e4-8686-e5023d266eee")
VoiceEnumerator;
#endif

EXTERN_C const CLSID CLSID_Logger;

#ifdef __cplusplus

class DECLSPEC_UUID("45aed94e-b97d-45ad-87a4-724b30fe69e1")
Logger;
#endif

EXTERN_C const CLSID CLSID_LogItem;

#ifdef __cplusplus

class DECLSPEC_UUID("74b45f12-fc01-4744-b55b-6d3b8f4b21a8")
LogItem;
#endif

EXTERN_C const CLSID CLSID_LogItems;

#ifdef __cplusplus

class DECLSPEC_UUID("f9c247fc-6045-497c-96dc-e5cc08729471")
LogItems;
#endif
#endif /* __TestTTSEngineLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



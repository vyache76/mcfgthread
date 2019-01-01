// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2019, LH_Mouse. All wrongs reserved.

#include "mcfcrt.h"
#include "env/thread.h"
#include "env/tls.h"
#include "env/mcfwin.h"

__attribute__((__stdcall__)) extern BOOL __MCFCRT_DllStartup(HINSTANCE hInstance, DWORD dwReason, LPVOID pReserved)
  __asm__("@__MCFCRT_DllStartup");

struct tagDllStartupParams
  {
    HINSTANCE hInstance;
    DWORD dwReason;
    LPVOID pReserved;
  }
typedef DllStartupParams;

static unsigned long WrappedDllStartup(void *pOpaque)
  {
    DllStartupParams *const pParams = pOpaque;

    switch(pParams->dwReason) {
      case DLL_PROCESS_ATTACH: {
        if(!__MCFCRT_InitRecursive()) {
          return false;
        }
        return true;
      }
      case DLL_PROCESS_DETACH: {
        __MCFCRT_TlsCleanup();
        __MCFCRT_UninitRecursive();
        return true;
      }
      case DLL_THREAD_ATTACH: {
        return true;
      }
      case DLL_THREAD_DETACH: {
        __MCFCRT_TlsCleanup();
        return true;
      }
      default: {
        return false;
      }
    }
  }

__attribute__((__stdcall__)) BOOL __MCFCRT_DllStartup(HINSTANCE hInstance, DWORD dwReason, LPVOID pReserved)
  {
    DllStartupParams vParams = { hInstance, dwReason, pReserved };
    const unsigned long dwResult = _MCFCRT_WrapThreadProcWithSehTop(&WrappedDllStartup, &vParams);
    return dwResult != 0;
  }

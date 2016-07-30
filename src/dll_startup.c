// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "env/mcfwin.h"
#include "env/module.h"
#include "env/thread.h"
#include "env/_seh_top.h"

__MCFCRT_C_STDCALL
BOOL __MCFCRT_DllStartup(HINSTANCE hDll, DWORD dwReason, LPVOID pReserved)
	__asm__("@__MCFCRT_DllStartup");

static bool RealStartup(void *pInstance, unsigned uReason, bool bDynamic){
	(void)pInstance;
	(void)bDynamic;

	bool bRet = true;

	switch(uReason){
	case DLL_PROCESS_ATTACH:
		bRet = __MCFCRT_ModuleInit();
		if(!bRet){
			goto jCleanup_00;
		}
		bRet = __MCFCRT_ThreadEnvInit();
		if(!bRet){
			goto jCleanup_01;
		}
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		__MCFCRT_TlsCleanup();
		break;

	case DLL_PROCESS_DETACH:
		__MCFCRT_TlsCleanup();
		__MCFCRT_ThreadEnvUninit();
	jCleanup_01:
		__MCFCRT_ModuleUninit();
	jCleanup_00:
		break;
	}

	return bRet;
}

__MCFCRT_C_STDCALL
BOOL __MCFCRT_DllStartup(HINSTANCE hDll, DWORD dwReason, LPVOID pReserved){
	bool bRet;

	void *const pInstance  = (void *)hDll;
	const unsigned uReason = dwReason;
	const bool bDynamic    = !pReserved;

	__MCFCRT_SEH_TOP_BEGIN
	{
		bRet = RealStartup(pInstance, uReason, bDynamic);
	}
	__MCFCRT_SEH_TOP_END

	return bRet;
}

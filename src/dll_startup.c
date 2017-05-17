// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2017, LH_Mouse. All wrongs reserved.

#include "mcfcrt.h"
#include "env/_seh_top.h"

__MCFCRT_C_STDCALL
extern BOOL __MCFCRT_DllStartup(HINSTANCE hInstance, DWORD dwReason, LPVOID pReserved)
	__asm__("@__MCFCRT_DllStartup");

extern bool __MCFCRT_Startup(void *__pModuleBase, unsigned __uReason, bool __bDynamic) _MCFCRT_NOEXCEPT;

__MCFCRT_C_STDCALL
BOOL __MCFCRT_DllStartup(HINSTANCE hInstance, DWORD dwReason, LPVOID pReserved){
	(void)hInstance;
	(void)pReserved;

	bool bRet = true;

	__MCFCRT_SEH_TOP_BEGIN
	{
		if(dwReason == DLL_PROCESS_ATTACH){
			bRet = __MCFCRT_Init();
		} else if(dwReason == DLL_PROCESS_DETACH){
			__MCFCRT_Uninit();
		}
	}
	__MCFCRT_SEH_TOP_END

	return bRet;
}

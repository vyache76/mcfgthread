// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "_eh_top.h"

__MCFCRT_C_STDCALL
long __MCFCRT_TopSehHandler(EXCEPTION_POINTERS *pContext){
	const DWORD dwCode = pContext->ExceptionRecord->ExceptionCode;
	const DWORD dwFlags = pContext->ExceptionRecord->ExceptionFlags;
	if((dwCode & 0x20FFFFFFul) == 0x20474343){ // ('GCC' | (1 << 29))
		if((dwFlags & EXCEPTION_NONCONTINUABLE) == 0){
			return EXCEPTION_CONTINUE_EXECUTION;
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

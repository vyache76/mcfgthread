// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2017, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_ENV_CRT_MODULE_H_
#define __MCFCRT_ENV_CRT_MODULE_H_

#include "_crtdef.h"

_MCFCRT_EXTERN_C_BEGIN

typedef void (*_MCFCRT_AtCrtModuleExitCallback)(_MCFCRT_STD intptr_t __nContext);

extern bool _MCFCRT_AtCrtModuleQuickExit(_MCFCRT_AtCrtModuleExitCallback __pfnProc, _MCFCRT_STD intptr_t __nContext) _MCFCRT_NOEXCEPT;

extern void __MCFCRT_DiscardCrtModuleQuickExitCallbacks(void) _MCFCRT_NOEXCEPT;

typedef enum __MCFCRT_tagExitType {
	_MCFCRT_kExitTypeNormal    = 0, // exit
	_MCFCRT_kExitTypeImmediate = 1, // _Exit
	_MCFCRT_kExitTypeQuick     = 2, // quick_exit
} _MCFCRT_ExitType;

__attribute__((__noreturn__))
extern void _MCFCRT_ExitProcess(unsigned __uExitCode, _MCFCRT_ExitType __eExitType) _MCFCRT_NOEXCEPT;

_MCFCRT_EXTERN_C_END

#endif

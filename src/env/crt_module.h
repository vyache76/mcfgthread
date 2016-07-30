// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_ENV_CRT_MODULE_H_
#define __MCFCRT_ENV_CRT_MODULE_H_

#include "../pre/module.h"

_MCFCRT_EXTERN_C_BEGIN

extern bool _MCFCRT_AtCrtModuleExit(_MCFCRT_AtModuleExitCallback __pfnProc, _MCFCRT_STD intptr_t __nContext) _MCFCRT_NOEXCEPT;

_MCFCRT_EXTERN_C_END

#endif

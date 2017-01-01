// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2017, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_ENV_BAIL_H_
#define __MCFCRT_ENV_BAIL_H_

#include "_crtdef.h"

_MCFCRT_EXTERN_C_BEGIN

__attribute__((__noreturn__, __cold__))
extern void _MCFCRT_Bail(const wchar_t *__pwszDescription) _MCFCRT_NOEXCEPT;

_MCFCRT_EXTERN_C_END

#endif

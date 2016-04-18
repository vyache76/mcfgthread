// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_ENV_NT_TIMEOUT_H_
#define __MCFCRT_ENV_NT_TIMEOUT_H_

#include "_crtdef.h"
#include "mcfwin.h"
#include "clocks.h"

__MCFCRT_EXTERN_C_BEGIN

static inline void __MCF_CRT_InitializeNtTimeout(LARGE_INTEGER *__pliTimeout, _MCFCRT_STD uint64_t __u64UntilFastMonoClock) _MCFCRT_NOEXCEPT {
	const _MCFCRT_STD uint64_t __u64Now = _MCFCRT_GetFastMonoClock();
	if(__u64UntilFastMonoClock < __u64Now){
		__pliTimeout->QuadPart = 0; // We should time out immediately.
		return;
	}
	const _MCFCRT_STD uint64_t __u64DeltaMs = __u64UntilFastMonoClock - __u64Now;
	if(__u64DeltaMs > INT64_MAX / 10000u){
		__pliTimeout->QuadPart = INT64_MAX; // We should never time out.
		return;
	}
	__pliTimeout->QuadPart = -(_MCFCRT_STD int64_t)(__u64DeltaMs * 10000u); // If this value is negative, the duration is mensured by the absolute value of it, in 100 nanoseconds.
}

__MCFCRT_EXTERN_C_END

#endif

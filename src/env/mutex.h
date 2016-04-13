// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_ENV_MUTEX_H_
#define __MCFCRT_ENV_MUTEX_H_

#include "_crtdef.h"

__MCFCRT_EXTERN_C_BEGIN

// In the case of static initialization, please initialize it with { 0 }.
typedef struct __MCFCRT_tagMutex {
	volatile _MCFCRT_STD uintptr_t __u;
} volatile _MCFCRT_Mutex;

#define _MCFCRT_MUTEX_SUGGESTED_SPIN_COUNT   100

static inline void _MCFCRT_InitializeMutex(_MCFCRT_Mutex *__pMutex) _MCFCRT_NOEXCEPT {
	__atomic_store_n(&(__pMutex->__u), 0, __ATOMIC_RELEASE);
}

extern bool _MCFCRT_WaitForMutex(_MCFCRT_Mutex *__pMutex, _MCFCRT_STD size_t __uMaxSpinCount, _MCFCRT_STD uint64_t __u64UntilFastMonoClock) _MCFCRT_NOEXCEPT;
extern void _MCFCRT_WaitForMutexForever(_MCFCRT_Mutex *__pMutex, _MCFCRT_STD size_t __uMaxSpinCount) _MCFCRT_NOEXCEPT;
extern void _MCFCRT_SignalMutex(_MCFCRT_Mutex *__pMutex) _MCFCRT_NOEXCEPT;

__MCFCRT_EXTERN_C_END

#endif

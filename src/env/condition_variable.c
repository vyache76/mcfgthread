// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2019, LH_Mouse. All wrongs reserved.

#define __MCFCRT_CONDITION_VARIABLE_INLINE_OR_EXTERN     extern inline
#include "condition_variable.h"
#include "_nt_timeout.h"
#include "xassert.h"
#include "expect.h"
#include <ntdef.h>

__attribute__((__dllimport__, __stdcall__)) extern NTSTATUS NtWaitForKeyedEvent(HANDLE hKeyedEvent, void *pKey, BOOLEAN bAlertable, const LARGE_INTEGER *pliTimeout);
__attribute__((__dllimport__, __stdcall__)) extern NTSTATUS NtReleaseKeyedEvent(HANDLE hKeyedEvent, void *pKey, BOOLEAN bAlertable, const LARGE_INTEGER *pliTimeout);

__attribute__((__dllimport__, __stdcall__, __const__)) extern BOOLEAN RtlDllShutdownInProgress(void);

#ifndef __BYTE_ORDER__
#  error Byte order is unknown.
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#  define BSUSR(v_)             ((uintptr_t)(v_) << CHAR_BIT)
#  define BSFB(v_)              ((uintptr_t)(v_)            )
#else
#  define BSUSR(v_)             ((uintptr_t)(v_)                                        )
#  define BSFB(v_)              ((uintptr_t)(v_) << ((sizeof(uintptr_t) - 1) * CHAR_BIT))
#endif

#define MASK_THREADS_TRAPPED    ((uintptr_t)(~BSFB(0xFF)))

#define SPIN_FAILURE_COUNT_ONE  ((uintptr_t)(MASK_SPIN_FAILURE_COUNT & -MASK_SPIN_FAILURE_COUNT))
#define SPIN_FAILURE_COUNT_MAX  ((uintptr_t)(MASK_SPIN_FAILURE_COUNT / SPIN_FAILURE_COUNT_ONE))

#define THREADS_TRAPPED_ONE     ((uintptr_t)(MASK_THREADS_TRAPPED & -MASK_THREADS_TRAPPED))
#define THREADS_TRAPPED_MAX     ((uintptr_t)(MASK_THREADS_TRAPPED / THREADS_TRAPPED_ONE))

static inline size_t Min(size_t uSelf, size_t uOther)
  {
    return (uSelf <= uOther) ? uSelf : uOther;
  }

__attribute__((__always_inline__)) static inline bool ReallyWaitForConditionVariable(volatile uintptr_t *puControl, _MCFCRT_ConditionVariableUnlockCallback pfnUnlockCallback, _MCFCRT_ConditionVariableRelockCallback pfnRelockCallback, intptr_t nContext, size_t uMaxSpinCountInitial, bool bMayTimeOut, uint64_t u64UntilFastMonoClock, bool bRelockIfTimeOut)
  {
    (void)uMaxSpinCountInitial;
    intptr_t nUnlocked;
    {
      uintptr_t uOld, uNew;
      uOld = __atomic_load_n(puControl, __ATOMIC_RELAXED);
      do {
        uNew = uOld + THREADS_TRAPPED_ONE;
      } while(_MCFCRT_EXPECT_NOT(!__atomic_compare_exchange_n(puControl, &uOld, uNew, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED)));
    }
    nUnlocked = (*pfnUnlockCallback)(nContext);
    if(bMayTimeOut) {
      LARGE_INTEGER liTimeout;
      __MCFCRT_InitializeNtTimeout(&liTimeout, u64UntilFastMonoClock);
      NTSTATUS lStatus = NtWaitForKeyedEvent(_MCFCRT_NULLPTR, (void *)puControl, false, &liTimeout);
      _MCFCRT_ASSERT_MSG(NT_SUCCESS(lStatus), L"NtWaitForKeyedEvent() failed.");
      while(_MCFCRT_EXPECT(lStatus == STATUS_TIMEOUT)) {
        bool bDecremented;
        {
          uintptr_t uOld, uNew;
          uOld = __atomic_load_n(puControl, __ATOMIC_RELAXED);
          do {
            const size_t uThreadsTrapped = (uOld & MASK_THREADS_TRAPPED) / THREADS_TRAPPED_ONE;
            bDecremented = uThreadsTrapped != 0;
            if(!bDecremented) {
              break;
            }
            uNew = uOld - THREADS_TRAPPED_ONE;
          } while(_MCFCRT_EXPECT_NOT(!__atomic_compare_exchange_n(puControl, &uOld, uNew, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED)));
        }
        if(bDecremented) {
          if(bRelockIfTimeOut) {
            (*pfnRelockCallback)(nContext, nUnlocked);
          }
          return false;
        }
        liTimeout.QuadPart = 0;
        lStatus = NtWaitForKeyedEvent(_MCFCRT_NULLPTR, (void *)puControl, false, &liTimeout);
        _MCFCRT_ASSERT_MSG(NT_SUCCESS(lStatus), L"NtWaitForKeyedEvent() failed.");
      }
    } else {
      NTSTATUS lStatus = NtWaitForKeyedEvent(_MCFCRT_NULLPTR, (void *)puControl, false, _MCFCRT_NULLPTR);
      _MCFCRT_ASSERT_MSG(NT_SUCCESS(lStatus), L"NtWaitForKeyedEvent() failed.");
      _MCFCRT_ASSERT(lStatus != STATUS_TIMEOUT);
    }
    (*pfnRelockCallback)(nContext, nUnlocked);
    return true;
  }

__attribute__((__always_inline__)) static inline size_t ReallySignalConditionVariable(volatile uintptr_t *puControl, size_t uMaxCountToReleaseOrSignal)
  {
    uintptr_t uCountToSignal; // Number of threads trapped to signal
    {
      uintptr_t uOld, uNew;
      uOld = __atomic_load_n(puControl, __ATOMIC_RELAXED);
      do {
        const size_t uThreadsTrapped = (uOld & MASK_THREADS_TRAPPED) / THREADS_TRAPPED_ONE;
        uCountToSignal = Min(uThreadsTrapped, uMaxCountToReleaseOrSignal);
        uNew = uOld - uCountToSignal * THREADS_TRAPPED_ONE;
        if(uNew == uOld) {
          break;
        }
      } while(_MCFCRT_EXPECT_NOT(!__atomic_compare_exchange_n(puControl, &uOld, uNew, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED)));
    }
    // If `RtlDllShutdownInProgress()` is `true`, other threads will have been terminated.
    // Calling `NtReleaseKeyedEvent()` when no thread is waiting results in deadlocks. Don't do that.
    if(_MCFCRT_EXPECT_NOT((uCountToSignal > 0) && !RtlDllShutdownInProgress())) {
      for(size_t uIndex = 0; uIndex < uCountToSignal; ++uIndex) {
        NTSTATUS lStatus = NtReleaseKeyedEvent(_MCFCRT_NULLPTR, (void *)puControl, false, _MCFCRT_NULLPTR);
        _MCFCRT_ASSERT_MSG(NT_SUCCESS(lStatus), L"NtReleaseKeyedEvent() failed.");
        _MCFCRT_ASSERT(lStatus != STATUS_TIMEOUT);
      }
    }
    return uCountToSignal;
  }

bool __MCFCRT_ReallyWaitForConditionVariable(_MCFCRT_ConditionVariable *pConditionVariable, _MCFCRT_ConditionVariableUnlockCallback pfnUnlockCallback, _MCFCRT_ConditionVariableRelockCallback pfnRelockCallback, intptr_t nContext, size_t uMaxSpinCount, uint64_t u64UntilFastMonoClock)
  {
    const bool bSignaled = ReallyWaitForConditionVariable(&(pConditionVariable->__u), pfnUnlockCallback, pfnRelockCallback, nContext, uMaxSpinCount, true, u64UntilFastMonoClock, true);
    return bSignaled;
  }

bool __MCFCRT_ReallyWaitForConditionVariableOrAbandon(_MCFCRT_ConditionVariable *pConditionVariable, _MCFCRT_ConditionVariableUnlockCallback pfnUnlockCallback, _MCFCRT_ConditionVariableRelockCallback pfnRelockCallback, intptr_t nContext, size_t uMaxSpinCount, uint64_t u64UntilFastMonoClock)
  {
    const bool bSignaled = ReallyWaitForConditionVariable(&(pConditionVariable->__u), pfnUnlockCallback, pfnRelockCallback, nContext, uMaxSpinCount, true, u64UntilFastMonoClock, false);
    return bSignaled;
  }

void __MCFCRT_ReallyWaitForConditionVariableForever(_MCFCRT_ConditionVariable *pConditionVariable, _MCFCRT_ConditionVariableUnlockCallback pfnUnlockCallback, _MCFCRT_ConditionVariableRelockCallback pfnRelockCallback, intptr_t nContext, size_t uMaxSpinCount)
  {
    const bool bSignaled = ReallyWaitForConditionVariable(&(pConditionVariable->__u), pfnUnlockCallback, pfnRelockCallback, nContext, uMaxSpinCount, false, UINT64_MAX, true);
    _MCFCRT_ASSERT(bSignaled);
  }

size_t __MCFCRT_ReallySignalConditionVariable(_MCFCRT_ConditionVariable *pConditionVariable, size_t uMaxCountToSignal)
  {
    return ReallySignalConditionVariable(&(pConditionVariable->__u), uMaxCountToSignal);
  }

size_t __MCFCRT_ReallyBroadcastConditionVariable(_MCFCRT_ConditionVariable *pConditionVariable)
  {
    return ReallySignalConditionVariable(&(pConditionVariable->__u), SIZE_MAX);
  }

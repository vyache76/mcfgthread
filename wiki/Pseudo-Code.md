# Pseudo Code

This page contains C-ish pseudo code of APIs of [[mutexes|env mutex.h]], [[condition variables|env condition_variable.h]] and [[once-initialization flags|env once_flag.h]].

## General Syntax

#### `ATOMIC_RELAXED { }`
#### `ATOMIC_ACQUIRE { }`
#### `ATOMIC_RELEASE { }`
#### `ATOMIC_ACQ_REL { }`

Execute a block of statements atomically. As for 2 and 4, entering the block has ACQUIRE semantics. As for 3 and 4, leaving the block has RELEASE semantics.

#### `PAUSE()`

Hint that the surrounding loop is a busy-waiting loop to help the CPU reduce power consumption and improve performance.

#### `WAIT_FOR_KEYED_EVENT(key, timeout)`

Wait for the global keyed event using the specified `key` and until `timeout`. This function shall only fail if timed out, in which case it returns `STATUS_TIMEOUT`. Any other errors cause runtime abort.

#### `RELEASE_KEYED_EVENT(key, timeout)`

Wake up another thread that is sleeping because of `RELEASE_KEYED_EVENT(key, timeout)` with the same `key` parameter. If there is no such thread to wake up, the function blocks the calling thread until `timeout`. This function shall only fail if it times out, in which case it returns `STATUS_TIMEOUT`. Any other errors cause runtime abort.

#### `IS_PROCESS_SHUTTING_DOWN()`

Return whether we are directly or indirectly inside a `DLL_PROCESS_DETACH` notification.

## Mutex

```c
typedef struct __MCFCRT_tagMutex {
  bool    bLocked;              // Has this mutex been locked by a thread?
  uintptr_t uThreadsSpinning;   // Number of threads waiting in user mode.
  uintptr_t uSpinFailureCount;  // Number of threads that failed to acquire
                                // the mutex inside their busy-waiting loops.
  uintptr_t uThreadsTrapped;    // Number of threads waiting or going to wait
                                // in kernel mode.
} _MCFCRT_Mutex;

void _MCFCRT_InitializeMutex(_MCFCRT_Mutex *pMutex) {
  ATOMIC_RELEASE {
    pMutex->bLocked       = false;
    pMutex->uThreadsSpinning  = 0;
    pMutex->uSpinFailureCount = 0;
    pMutex->uThreadsTrapped   = 0;
  }
}

bool _MCFCRT_WaitForMutex(_MCFCRT_Mutex *pMutex, size_t uMaxSpinCount,
  uint64_t u64UntilFastMonoClock)
{
  for(;;) {
    size_t uRealMaxSpinCount = 0;  // This may be less than `uMaxSpinCount`.
    bool bSpinnable = false;       // Can I spin for a while before invoking
                                   // a system call to sleep?
    ATOMIC_ACQUIRE {
      if(!pMutex->bLocked) {
        // The mutex has not been locked. Lock it.
        pMutex->bLocked = true;
        if(pMutex->uSpinFailureCount > 0) {
          pMutex->uSpinFailureCount -= 1;
        }
        return true;
      }
      // Guess the max spin count.
      if(uMaxSpinCount > MIN_SPIN_COUNT) {
        uRealMaxSpinCount = MIN_SPIN_COUNT |
                            (uMaxSpinCount >> pMutex->uSpinFailureCount);
      } else {
        uRealMaxSpinCount = uMaxSpinCount;
      }
      if(uRealMaxSpinCount && (pMutex->uThreadsSpinning < MAX_THREADS_SPINNING)) {
        // Increment the counter and prepare for spinning.
        pMutex->uThreadsSpinning += 1;
        bSpinnable = true;
      }
    }
    if(bSpinnable) {
      // Spin for a while.
      // Remember to decrement the number of threads spinning in this branch!
      for(size_t uSpinIndex = 0; uSpinIndex < uRealMaxSpinCount; ++uSpinIndex) {
        ATOMIC_ACQUIRE {
          if(!pMutex->bLocked) {
            pMutex->bLocked = true;
            pMutex->uThreadsSpinning -= 1;
            if(pMutex->uSpinFailureCount > 0) {
              pMutex->uSpinFailureCount -= 1;
            }
            return true;
          }
        }
        PAUSE();
      }
      // Request to sleep in kernel mode.
      ATOMIC_ACQUIRE {
        if(!pMutex->bLocked) {
          pMutex->bLocked = true;
          pMutex->uThreadsSpinning -= 1;
          if(pMutex->uSpinFailureCount > 0) {
            pMutex->uSpinFailureCount -= 1;
          }
          return true;
        }
        pMutex->uThreadsSpinning -= 1;
        // Tell another thread which is going to unlock this mutex that there
        // is a new waiter by incrementing the number of threads trapped.
        // Note that all this requires there be such a thread (in other words,
        // the mutex must have been locked), otherwise we get a deadlock
        // in this thread.
        pMutex->uThreadsTrapped += 1;
        if(pMutex->uSpinFailureCount < SPIN_FAILURE_COUNT_MAX) {
          pMutex->uSpinFailureCount += 1;
        }
      }
    } else {
      // Request to sleep in kernel mode.
      ATOMIC_ACQUIRE {
        if(!pMutex->bLocked) {
          pMutex->bLocked = true;
          return true;
        }
        // The same as above.
        pMutex->uThreadsTrapped += 1;
      }
    }
    NTSTATUS lStatus = WAIT_FOR_KEYED_EVENT(pMutex, u64UntilFastMonoClock);
    while(lStatus == STATUS_TIMEOUT) {
      ATOMIC_ACQUIRE {
        // Tell another thread which is going to unlock this mutex that
        // an old waiter has left by decrementing the number of threads
        // trapped. But see below...
        if(pMutex->uThreadsTrapped > 0) {
          pMutex->uThreadsTrapped -= 1;
          return false;
        }
      }
      // ... It is possible that the second thread has already decremented
      // the counter. If it does take place, the second thread is going to
      // RELEASE_KEYED_EVENT() soon. We must wait again, otherwise we get
      // a deadlock in the second thread. Again, a third thread could
      // WAIT_FOR_KEYED_EVENT() before us, so we set the timeout to zero.
      // In the case of timing out, the third thread will have incremented
      // the number of threads trapped and we can try decrementing it again.
      lStatus = WAIT_FOR_KEYED_EVENT(pMutex, INFINITE_IN_THE_PAST);
    }
    // Retry locking the mutex.
  }
}
void _MCFCRT_SignalMutex(_MCFCRT_Mutex *pMutex) {
  bool bSignalOne = false;  // Is there at least one other thread waiting in
                            // kernel mode and should be woken up?
  ATOMIC_RELEASE {
    pMutex->bLocked = false;
    if(pMutex->uThreadsTrapped > 0) {
      pMutex->uThreadsTrapped -= 1;
      bSignalOne = true;
    }
  }
  // A call to ExitProcess() terminates any other threads, even if they are waiting
  // for a mutex. Don't RELEASE_KEYED_EVENT() in this case because it blocks
  // the calling thread infinitely if there is no other thread to wake up.
  // See <https://github.com/lhmouse/mcfgthread/issues/21>.
  if(bSignalOne && !IS_PROCESS_SHUTTING_DOWN()) {
    RELEASE_KEYED_EVENT(pMutex, INFINITE_IN_THE_FUTURE);
  }
}
```

## Condition Variable

```c
typedef struct __MCFCRT_tagConditionVariable {
  uintptr_t uThreadsTrapped;    // Number of threads waiting or going to wait
                                // in kernel mode.
} _MCFCRT_ConditionVariable;

void _MCFCRT_InitializeConditionVariable(_MCFCRT_ConditionVariable *pCondVar) {
  ATOMIC_RELEASE {
    pCondVar->uThreadsTrapped   = 0;
  }
}

bool _MCFCRT_WaitForConditionVariable(_MCFCRT_ConditionVariable *pCondVar,
  _MCFCRT_ConditionVariableUnlockCallback pfnUnlockCallback,
  _MCFCRT_ConditionVariableRelockCallback pfnRelockCallback, intptr_t nContext,
  size_t uMaxSpinCount, uint64_t u64UntilFastMonoClock)
{
  // Request to sleep in kernel mode.
  ATOMIC_RELAXED {
    // Tell another thread which is going to signal this condition variable
    // that there is a new waiter by incrementing the number of threads
    // trapped.
    pCondVar->uThreadsTrapped += 1;
  }
  // We are going to wait, so unlock the mutex first.
  intptr_t nUnlocked = (*pfnUnlockCallback)(nContext);
  NTSTATUS lStatus = WAIT_FOR_KEYED_EVENT(pCondVar, u64UntilFastMonoClock);
  while(lStatus == STATUS_TIMEOUT) {
    ATOMIC_RELAXED {
      // Tell another thread which is going to signal this condition variable
      // that an old waiter has left by decrementing the number of threads
      // trapped. But see below...
      if(pCondVar->uThreadsTrapped > 0) {
        pCondVar->uThreadsTrapped -= 1;
        // Relock the mutex normally, except for
        // `_MCFCRT_WaitForConditionVariableOrAbandon()`.
        (*pfnRelockCallback)(nContext, nUnlocked);
        return false;
      }
    }
    // ... It is possible that the second thread has already decremented
    // the counter. If it does take place, the second thread is going to
    // RELEASE_KEYED_EVENT() soon. We must wait again, otherwise we get a deadlock
    // in the second thread. Again, a third thread could WAIT_FOR_KEYED_EVENT()
    // before us, so we set the timeout to zero. In the case of timing out,
    // the third thread will have incremented the number of threads trapped and
    // we can try decrementing it again.
    lStatus = WAIT_FOR_KEYED_EVENT(pCondVar, INFINITE_IN_THE_PAST);
  }
  // Condition variables are unreliable.
  (*pfnRelockCallback)(nContext, nUnlocked);
  return true;
}
size_t _MCFCRT_SignalConditionVariable(_MCFCRT_ConditionVariable *pCondVar,
  size_t uMaxCountToSignal)
{
  uintptr_t uCountToSignal; // Number of threads to wake up from sleep.
  ATOMIC_RELAXED {
    const size_t uThreadsTrapped = pCondVar->uThreadsTrapped;
    uCountToSignal = MIN(uThreadsTrapped, uMaxCountToReleaseOrSignal);
    pCondVar->uThreadsTrapped -= uCountToSignal;
  }
  // A call to ExitProcess() terminates any other threads, even if they are waiting
  // for a condition variable. Don't RELEASE_KEYED_EVENT() in this case because
  // it blocks the calling thread infinitely if there is no other thread to wake up.
  // See <https://github.com/lhmouse/mcfgthread/issues/21>.
  if(uCountToSignal && !IS_PROCESS_SHUTTING_DOWN()) {
    for(size_t uIndex = 0; uIndex < uCountToSignal; ++uIndex) {
      RELEASE_KEYED_EVENT(pCondVar, INFINITE_IN_THE_FUTURE);
    }
  }
  return uCountToSignal;
}
```

## Once-initialization

```c
typedef struct __MCFCRT_tagOnceFlag {
  bool    bFinished;          // Has the initialization completed successfully?
  bool    bLocked;            // Is there a thread performing initialization?
  uintptr_t uThreadsTrapped;  // Number of threads waiting or going to wait
                              // in kernel mode.
} _MCFCRT_OnceFlag;

void _MCFCRT_InitializeOnceFlag(_MCFCRT_OnceFlag *pOnceFlag) {
  ATOMIC_RELEASE {
    pOnceFlag->bFinished    = false;
    pOnceFlag->bLocked      = false;
    pOnceFlag->uThreadsTrapped  = 0;
  }
}

_MCFCRT_OnceResult _MCFCRT_WaitForOnceFlag(_MCFCRT_OnceFlag *pOnceFlag,
  uint64_t u64UntilFastMonoClock)
{
  for(;;) {
    bool bSpinnable = false; // Can I spin for a while before invoking a system
                             // call to sleep?
    ATOMIC_ACQUIRE {
      if(pOnceFlag->bFinished) {
        return _MCFCRT_kOnceResultFinished;
      }
      // Try locking the once flag as if it were a mutex.
      if(!pOnceFlag->bLocked) {
        // The once flag has not been locked. Lock it.
        pOnceFlag->bLocked = true;
        return _MCFCRT_kOnceResultInitial;
      }
      // Tell another thread which is going to unlock this once flag that
      // there is a new waiter by incrementing the number of threads trapped.
      // Note that all this requires there be such a thread (in other words,
      // the once flag must have been locked), otherwise we get a deadlock
      // in this thread.
      // For simplification of design, once flags don't support spinning.
      pOnceFlag->uThreadsTrapped += 1;
    }
    NTSTATUS lStatus = WAIT_FOR_KEYED_EVENT(pOnceFlag, u64UntilFastMonoClock);
    while(lStatus == STATUS_TIMEOUT) {
      ATOMIC_ACQUIRE {
        // Tell another thread which is going to unlock this once flag that
        // an old waiter has left by decrementing the number of threads
        // trapped. But see below...
        if(pOnceFlag->uThreadsTrapped > 0) {
          pOnceFlag->uThreadsTrapped -= 1;
          return _MCFCRT_kOnceResultTimedOut;
        }
      }
      // ... It is possible that the second thread has already decremented
      // the counter. If it does take place, the second thread is going to
      // RELEASE_KEYED_EVENT() soon. We must wait again, otherwise we get
      // a deadlock in the second thread. Again, a third thread could
      // WAIT_FOR_KEYED_EVENT() before us, so we set the timeout to zero.
      // In the case of timing out, the third thread will have incremented
      // the number of threads trapped and we can try decrementing it again.
      lStatus = WAIT_FOR_KEYED_EVENT(pOnceFlag, INFINITE_IN_THE_PAST);
    }
    // Retry waiting for the once flag.
  }
}
void _MCFCRT_SignalOnceFlagAsFinished(_MCFCRT_OnceFlag *pOnceFlag) {
  uintptr_t uCountToSignal; // How many threads should we wake up from kernel mode?
  ATOMIC_RELEASE {
    pOnceFlag->bLocked = false;
    pOnceFlag->bFinished = true;
    uCountToSignal = pOnceFlag->uThreadsTrapped;
    pOnceFlag->uThreadsTrapped = 0;
  }
  // A call to ExitProcess() terminates any other threads, even if they are waiting
  // for a once flag. Don't RELEASE_KEYED_EVENT() in this case because it blocks
  // the calling thread infinitely if there is no other thread to wake up.
  // See <https://github.com/lhmouse/mcfgthread/issues/21>.
  if(uCountToSignal && !IS_PROCESS_SHUTTING_DOWN()) {
    for(size_t uIndex = 0; uIndex < uCountToSignal; ++uIndex) {
      RELEASE_KEYED_EVENT(pOnceFlag, INFINITE_IN_THE_FUTURE);
    }
  }
}
void _MCFCRT_SignalOnceFlagAsAborted(_MCFCRT_OnceFlag *pOnceFlag) {
  bool bSignalOne = false;  // Is there at least one other thread waiting in
                            // kernel mode and should be woken up?
  ATOMIC_RELEASE {
    pOnceFlag->bLocked = false;
    if(pOnceFlag->uThreadsTrapped > 0) {
      pOnceFlag->uThreadsTrapped -= 1;
      bSignalOne = true;
    }
  }
  // A call to ExitProcess() terminates any other threads, even if they are waiting
  // for a once flag. Don't RELEASE_KEYED_EVENT() in this case because it blocks
  // the calling thread infinitely if there is no other thread to wake up.
  // See <https://github.com/lhmouse/mcfgthread/issues/21>.
  if(bSignalOne && !IS_PROCESS_SHUTTING_DOWN()) {
    RELEASE_KEYED_EVENT(pOnceFlag, INFINITE_IN_THE_FUTURE);
  }
}
```

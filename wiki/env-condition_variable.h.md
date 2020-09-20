# env/condition_variable.h

## Macros

#### _\(Since 2.x\)_ `#define _MCFCRT_CONDITION_VARIABLE_SUGGESTED_SPIN_COUNT   1000u`

* **Description**

    This is the suggested spin count when calling `_MCFCRT_WaitForConditionVariable()`, `_MCFCRT_WaitForConditionVariableOrAbandon()` or `_MCFCRT_WaitForConditionVariableForever()`.  
    There is no silver bullet. Thus, it is suggested that experienced users pick more appropriate values when calling these functions.

## Types

#### `typedef struct __MCFCRT_tagConditionVariable _MCFCRT_ConditionVariable;`

* **Description**

    This is an opaque `struct` representing a condition variable.  
    The user must treat the `struct` opaque and make no assumption of the bit representation of it.

#### `typedef intptr_t (*_MCFCRT_ConditionVariableUnlockCallback)(intptr_t nContext);`
#### `typedef void (*_MCFCRT_ConditionVariableRelockCallback)(intptr_t nContext, intptr_t nUnlocked);`

* **Description**

    These are types for callbacks when a thread is about to be suspended on a condition variable and when a thread is just woken up from a condition variable, respectively.

## Functions

#### `void _MCFCRT_InitializeConditionVariable(_MCFCRT_ConditionVariable *pConditionVariable);`

* **Description**

    This function initializes a condition variable. This function has release semantics.  
    If another thread is suspended on this condition variable, the behavior is undefined.

* **Parameters**

    `_MCFCRT_ConditionVariable *pConditionVariable`  
    The condition variable to initialize. This parameter must not be `NULL`.

* **Return value**

    This function does not return a value.

* **Remarks**

    To initialize a condition variable statically, use a brace-enclosed list of a single element of `0` like the following:  

    `_MCFCRT_ConditionVariable my_cond = { 0 };`  

#### _\(Until 2.x\)_ `bool _MCFCRT_WaitForConditionVariable(_MCFCRT_ConditionVariable *pConditionVariable, _MCFCRT_ConditionVariableUnlockCallback pfnUnlockCallback, _MCFCRT_ConditionVariableRelockCallback pfnRelockCallback, intptr_t nContext, uint64_t u64UntilFastMonoClock);`

#### _\(Since 2.x\)_ `bool _MCFCRT_WaitForConditionVariable(_MCFCRT_ConditionVariable *pConditionVariable, _MCFCRT_ConditionVariableUnlockCallback pfnUnlockCallback, _MCFCRT_ConditionVariableRelockCallback pfnRelockCallback, intptr_t nContext, size_t uMaxSpinCount, uint64_t u64UntilFastMonoClock);`

* **Description**

    This function suspends the execution of the current thread until the specified time point is exceeded or another thread calls one of the wakeup functions.  
    Before the thread is suspended, it calls `(*pfnUnlockCallback)(nContext)` and stores its returned `intptr_t` in, let's say, `nUnlocked`. When the thread is woken up, either because the time point has been exceeded or another thread has called a wakeup function, it calls `(*pfnRelockCallback)(nContext, nUnlocked)` before returning from this function.  
_\(Since 2.x\)_ Because of spurious wakeups, it is possible for the calling thread to return immediately. In that case neither callback is invoked. 

* **Parameters**

    `_MCFCRT_ConditionVariable *pConditionVariable`  
    The condition variable to wait for.  

    `_MCFCRT_ConditionVariableUnlockCallback pfnUnlockCallback`  
    The callback to be called before the thread is suspended.  

    `_MCFCRT_ConditionVariableRelockCallback pfnRelockCallback`  
    The callback to be called after the thread is woken up.  

    `intptr_t nContext`  
    Common user-defined parameter for both callbacks.  

    `u64UntilFastMonoClock`  
    If the value of the fast monotonic clock exceeds this value, the wait is abandoned and the current thread resumes execution with this function returning `false`.  
    This value shall be offset from the return value of `_MCFCRT_GetFastMonoClock()`.

_\(Since 2.x\)_ `size_t uMaxSpinCount`  
    Before a thread requests the kernel to suspend itself, it tries spinning for a while which saves a system call if another thread would signal this condition variable very soon. This parameter specifies how long, in number of loops, the thread should spin for.

* **Return value**

    This function returns `false` if the time point is exceeded and `true` otherwise.

* **Remarks**

    Note that condition variables are subject to [spurious wakeups](https://en.wikipedia.org/wiki/Spurious_wakeup).  
    This function does NOT guarantee a first in, first out order.

#### _\(Since 2.x\)_ `bool _MCFCRT_WaitForConditionVariableOrAbandon(_MCFCRT_ConditionVariable *pConditionVariable, _MCFCRT_ConditionVariableUnlockCallback pfnUnlockCallback, _MCFCRT_ConditionVariableRelockCallback pfnRelockCallback, intptr_t nContext, size_t uMaxSpinCount, uint64_t u64UntilFastMonoClock);`

* **Description**

    This function is identical to `_MCFCRT_WaitForConditionVariable()` except that it does not call `pfnRelockCallback` if it returns `false`. That is, `pfnRelockCallback` is not called if the wait operation times out.

* **Parameters**

    `_MCFCRT_ConditionVariable *pConditionVariable`  
    The condition variable to wait for.  

    `_MCFCRT_ConditionVariableUnlockCallback pfnUnlockCallback`  
    The callback to be called before the thread is suspended.  

    `_MCFCRT_ConditionVariableRelockCallback pfnRelockCallback`  
    The callback to be called after the thread is woken up.  

    `intptr_t nContext`  
    Common user-defined parameter for both callbacks.  

    `u64UntilFastMonoClock`  
    If the value of the fast monotonic clock exceeds this value, the wait is abandoned and the current thread resumes execution with this function returning `false`.  
    This value shall be offset from the return value of `_MCFCRT_GetFastMonoClock()`.

    `size_t uMaxSpinCount`  
    Before a thread requests the kernel to suspend itself, it tries spinning for a while which saves a system call if another thread would signal this condition variable very soon. This parameter specifies how long, in number of loops, the thread should spin for.

* **Return value**

    This function returns `false` if the time point is exceeded and `true` otherwise.

#### _\(Until 2.x\)_ `void _MCFCRT_WaitForConditionVariableForever(_MCFCRT_ConditionVariable *pConditionVariable, _MCFCRT_ConditionVariableUnlockCallback pfnUnlockCallback, _MCFCRT_ConditionVariableRelockCallback pfnRelockCallback, intptr_t nContext);`

#### _\(Since 2.x\)_ `void _MCFCRT_WaitForConditionVariableForever(_MCFCRT_ConditionVariable *pConditionVariable, _MCFCRT_ConditionVariableUnlockCallback pfnUnlockCallback, _MCFCRT_ConditionVariableRelockCallback pfnRelockCallback, intptr_t nContext, size_t uMaxSpinCount);`

* **Description**

    This function is an efficient version of `_MCFCRT_WaitForConditionVariable()` that never times out hence does not return a value.

* **Parameters**

    `_MCFCRT_ConditionVariable *pConditionVariable`  
    The condition variable to wait for.  

    `_MCFCRT_ConditionVariableUnlockCallback pfnUnlockCallback`  
    The callback to be called before the thread is suspended.  

    `_MCFCRT_ConditionVariableRelockCallback pfnRelockCallback`  
    The callback to be called after the thread is woken up.  

    `intptr_t nContext`  
    Common user-defined parameter for both callbacks.  

_\(Since 2.x\)_ `size_t uMaxSpinCount`  
    Before a thread requests the kernel to suspend itself, it tries spinning for a while which saves a system call if another thread would signal this condition variable very soon. This parameter specifies how long, in number of loops, the thread should spin for.

* **Return value**

    This function does not return a value.

#### `size_t _MCFCRT_SignalConditionVariable(_MCFCRT_ConditionVariable *pConditionVariable, size_t uMaxCountToSignal);`

* **Description**

    This function wakes up no more than `uMaxCountToSignal` threads that have been suspended on this condition variable.

* **Parameters**

    `_MCFCRT_ConditionVariable *pConditionVariable`  
    The condition variable which other threads might be suspended on.  

    `size_t uMaxCountToSignal`  
    Max number of threads to wake up.

* **Return value**

    The return value is the number of threads that have been woken up from kernel mode. This value may be less than the actual number of threads woken up on the condition variable because of spurious wakeups.  
    Particularly, if no thread was suspended on the condition variable before the call, the return value is zero.

#### `size_t _MCFCRT_BroadcastConditionVariable(_MCFCRT_ConditionVariable *pConditionVariable);`

* **Description**

    This function is an efficient version of `_MCFCRT_SignalConditionVariable()` with the `uMaxCountToSignal` parameter set to `SIZE_MAX`.

* **Parameters**

    `_MCFCRT_ConditionVariable *pConditionVariable`  
    The condition variable which other threads might be suspended on.  

* **Return value**

    The return value is the number of threads that have been woken up.  

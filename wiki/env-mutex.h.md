# env/mutex.h

## Macros

#### `#define _MCFCRT_MUTEX_SUGGESTED_SPIN_COUNT   100u`

* **Description**

    This is the suggested spin count when calling `_MCFCRT_WaitForMutex()` or `_MCFCRT_WaitForMutexForever()`.  
    There is no silver bullet. Thus, it is suggested that experienced users pick more appropriate values when calling these functions.

## Types

#### `typedef struct __MCFCRT_tagMutex _MCFCRT_Mutex;`

* **Description**

    This is an opaque `struct` representing a mutex.  
    The user must treat the `struct` opaque and make no assumption of the bit representation of it.

## Functions

#### `void _MCFCRT_InitializeMutex(_MCFCRT_Mutex *pMutex);`

* **Description**

    This function initializes a mutex. This function has release semantics.  
    If another thread is waiting for this mutex, the behavior is undefined.

* **Parameters**

    `_MCFCRT_Mutex *pMutex`  
    The mutex to initialize. This parameter must not be `NULL`.

* **Return value**

    This function does not return a value.

* **Remarks**

    To initialize a mutex statically, use a brace-enclosed list of a single element of `0` like the following:  

    `_MCFCRT_Mutex my_mutex = { 0 };`  

#### `bool _MCFCRT_WaitForMutex(_MCFCRT_Mutex *pMutex, size_t uMaxSpinCount, uint64_t u64UntilFastMonoClock);`

* **Description**

    Each mutex can be in either of the two states: _unlocked_ and _locked_.  A mutex is initialized to the _unlocked_ state.  
    When a thread calls `_MCFCRT_WaitForMutex()` on a mutex that is _unlocked_, it changes that mutex to the _locked_ state and causes this function to return `true` immediately.  
    When a thread calls `_MCFCRT_WaitForMutex()` on a mutex that is _locked_, it is suspended on that mutex until the specified time point is exceeded or another thread resets the mutex to the _unlocked_ state. In the former case this function returns `false`. In the latter case contention for the mutex is repeated, and only if it is the current thread that changes the mutex from the _locked_ state to the _unlocked_ state can the current thread resume execution.

* **Parameters**

    `_MCFCRT_Mutex *pMutex`  
    The mutex to try to lock.  

    `size_t uMaxSpinCount`  
    If a thread can't lock this mutex (because it is already locked by another thread), before it requests the kernel to suspend itself, it tries spinning for a while which saves a system call if another thread would unlock this mutex very soon. This parameter specifies how long, in number of loops, the thread should spin for.  

    `u64UntilFastMonoClock`  
    If the value of the fast monotonic clock exceeds this value, the wait is abandoned and the current thread resumes execution with this function returning `false`.  
    This value shall be offset from the return value of `_MCFCRT_GetFastMonoClock()`.

* **Return value**

    This function returns `true` if the mutex is locked by the current thread and `false` otherwise.

* **Remarks**

    This function does NOT guarantee a first in, first out order.

#### `void _MCFCRT_WaitForMutexForever(_MCFCRT_Mutex *pMutex, size_t uMaxSpinCount);`

* **Description**

    This function is an efficient version of `_MCFCRT_WaitForMutex()` that never times out hence does not return a value.

* **Parameters**

    `_MCFCRT_Mutex *pMutex`  
    The mutex to try to lock.  

    `size_t uMaxSpinCount`  
    If a thread can't lock this mutex (because it is already locked by another thread), before it requests the kernel to suspend itself, it tries spinning for a while which saves a system call if another thread would unlock this mutex very soon. This parameter specifies how long, in number of loops, the thread should spin for.  

* **Return value**

    This function does not return a value.

#### `void _MCFCRT_SignalMutex(_MCFCRT_Mutex *pMutex);`

* **Description**

    This function sets the mutex to the _unlocked_ state and wakes up another thread that has been suspended on this mutex, if any.  
    Unlocking a mutex in a thread other than the thread that has locked the mutex is explicitly permitted. This characteristic is required by shared mutexes such as [readers-writer locks](https://en.wikipedia.org/wiki/Readers–writer_lock).  
    If the mutex is not in a _locked_ state, the behavior is undefined.

* **Parameters**

    `_MCFCRT_Mutex *pMutex`  
    The mutex that has been locked, either by the current thread or another thread.

* **Return value**

    This function does not return a value.

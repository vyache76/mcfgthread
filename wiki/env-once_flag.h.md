# env/once_flag.h

## Macros

None.

## Types

#### `typedef struct __MCFCRT_tagOnceFlag _MCFCRT_OnceFlag;`

* **Description**

    This is an opaque `struct` representing a once-initialization flag.  
    The bit representation of this `struct` is conforming to the Itanium ABI. The size of this `struct` is at most 64 bits. The first byte is set to `0` when this once-initialization flag is not _disposed_ and to `1` when this once-initialization flag has been _disposed_.  
    The user must treat the `struct` otherwise opaque and make no assumption of the bit representation of any other part but the first byte of it.

#### `typedef enum __MCFCRT_tagOnceResult _MCFCRT_OnceResult;`

* **Description**

    This enumeration indicates what action `_MCFCRT_WaitForOnceFlag()` or `_MCFCRT_WaitForOnceFlagForever()` suggests the user to do after it returns.

## Enumeration Constants

* **Description**

    ```c
    enum {
        _MCFCRT_kOnceResultTimedOut = 1,
        _MCFCRT_kOnceResultInitial  = 2,
        _MCFCRT_kOnceResultFinished = 3,
    };
    ```

## Functions

#### `_MCFCRT_InitializeOnceFlag(_MCFCRT_OnceFlag *pOnceFlag);`

* **Description**

    This function initializes a once-initialization flag. This function has release semantics.  
    If another thread is waiting for this once-initialization flag, the behavior is undefined.

* **Parameters**

    `_MCFCRT_OnceFlag *pOnceFlag`  
    The once-initialization flag to initialize. This parameter must not be `NULL`.

* **Return value**

    This function does not return a value.

* **Remarks**

    To initialize a once-initialization flag statically, use a brace-enclosed list of a single element of `0` like the following:  

    `_MCFCRT_OnceFlag my_once = { 0 };`  

#### `_MCFCRT_OnceResult _MCFCRT_WaitForOnceFlag(_MCFCRT_OnceFlag *pOnceFlag, uint64_t u64UntilFastMonoClock);`

* **Description**

    Each once-initialization flag can be in any of the three states: _unlocked_, _locked_ and _disposed_.  An once-initialization flag is initialized to the _unlocked_ state.  
    When a thread calls `_MCFCRT_WaitForOnceFlag()` on a once-initialization flag that is _unlocked_, it changes that once-initialization flag to the _locked_ state and causes this function to return `_MCFCRT_kOnceResultInitial` immediately. The user is responsible for initialization of shared resources upon receival of such a return value.  
    When a thread calls `_MCFCRT_WaitForOnceFlag()` on a once-initialization flag that is _locked_, it is suspended on that once-initialization flag until the specified time point is exceeded or another thread sets the once-initialization flag to the _unlocked_ or _disposed_ state. In the former case this function returns `_MCFCRT_kOnceResultTimedOut`. In the latter case, if the once-initialization flag is set to the _unlocked_ state, contention for the once-initialization flag is repeated; if the once-initialization flag is set to the _disposed_ state, no contention takes place anymore and this function returns `_MCFCRT_kOnceResultFinished`.  
    When a thread calls `_MCFCRT_WaitForOnceFlag()` on a once-initialization that is _disposed_, the function does nothing and `_MCFCRT_kOnceResultFinished` is returned immediately.

* **Parameters**

    `_MCFCRT_OnceFlag *pOnceFlag`  
    The once-initialization flag to wait for.  

    `u64UntilFastMonoClock`  
    If the value of the fast monotonic clock exceeds this value, the wait is abandoned and the current thread resumes execution with this function returning `_MCFCRT_kOnceResultTimedOut`.  
    This value shall be offset from the return value of `_MCFCRT_GetFastMonoClock()`.

* **Return value**

    This function returns `_MCFCRT_kOnceResultTimedOut` if the specified time point has exceeded, `_MCFCRT_kOnceResultInitial` if the once-initialization flag has been locked by the current thread, and `_MCFCRT_kOnceResultFinished` if the once-initialization flag has been disposed by another thread.

#### `_MCFCRT_OnceResult _MCFCRT_WaitForOnceFlagForever(_MCFCRT_OnceFlag *pOnceFlag);`

* **Description**

    This function is an efficient version of `_MCFCRT_WaitForOnceFlag()` that never times out hence never returns `_MCFCRT_kOnceResultTimedOut`.

* **Parameters**

    `_MCFCRT_OnceFlag *pOnceFlag`  
    The once-initialization flag to wait for.   

* **Return value**

    This function returns `_MCFCRT_kOnceResultInitial` if the once-initialization flag has been locked by the current thread, and `_MCFCRT_kOnceResultFinished` if the once-initialization flag has been disposed by another thread.

#### `void _MCFCRT_SignalOnceFlagAsFinished(_MCFCRT_OnceFlag *pOnceFlag);`

* **Description**

    This function sets the once-initialization flag to the _disposed_ state and wakes up all threads that have been suspended on this once-initialization flag, if any.  
    Once a once-initialization flag is set to the _disposed_ state there is no way to set it to any other state, except by re-initializing it, which semantically destroys the original once-initialization flag and creates a new one.  
    Unlocking a once-initialization flag in a thread other than the thread that has locked the once-initialization flag is rare but permitted usage.  
    If the once-initialization flag is not in a _locked_ state, the behavior is undefined.

* **Parameters**

    `_MCFCRT_OnceFlag *pOnceFlag`  
    The once-initialization flag that has been locked, either by the current thread or any other thread.

* **Return value**

    This function does not return a value.

#### `void _MCFCRT_SignalOnceFlagAsAborted(_MCFCRT_OnceFlag *pOnceFlag);`

* **Description**

    This function resets the once-initialization flag to the _unlocked_ state and wakes up another thread that has been suspended on this once-initialization flag, if any.  
    Unlocking a once-initialization flag in a thread other than the thread that has locked the once-initialization flag is rare but permitted usage.  
    If the once-initialization flag is not in a _locked_ state, the behavior is undefined.

* **Parameters**

    `_MCFCRT_OnceFlag *pOnceFlag`  
    The once-initialization flag that has been locked, either by the current thread or another thread.

* **Return value**

    This function does not return a value.

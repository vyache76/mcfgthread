# env/thread.h

## Macros

None.

## Types

#### `typedef unsigned long (*__stdcall _MCFCRT_NativeThreadProc)(void *pParam);`

* **Description**

    This is the type for the callback for a new thread created by `_MCFCRT_CreateNativeThread()`.

#### _\(Since 2.x\)_ `typedef unsigned long (*_MCFCRT_WrappedThreadProc)(void *pParam);`

* **Description**

    This is the type for a thread procedure that should be wrapped with an SEH handler.

#### `typedef void (*_MCFCRT_TlsDestructor)(intptr_t nContext, void *pStorage);`

* **Description**

    This is the type for the callback for a new thread created by `_MCFCRT_CreateThread()`.

#### `typedef struct __MCFCRT_tagThreadHandle *_MCFCRT_ThreadHandle;`

* **Description**

    This is an opaque pointer type playing a role as the handle of a thread.  

## Functions

#### `_MCFCRT_ThreadHandle _MCFCRT_CreateNativeThread(_MCFCRT_NativeThreadProc pfnThreadProc, void *pParam, bool bSuspended, uintptr_t *restrict puThreadId);`

* **Description**

    This function creates a new thread. The new thread begins execution as if it evaluates the expression `(*pfnThreadProc)(pParam)`.  
    The thread callback is not guaranteed a fully aligned [stack pointer](https://en.wikipedia.org/wiki/Call_stack#STACK-POINTER) upon start of execution on x86, which may cause segment faults if [SSE](https://en.wikipedia.org/wiki/Streaming_SIMD_Extensions) or `-ffast-math` is enabled. Therefore, we suggest you mark the callback as `__attribute__((__force_align_arg_pointer__))` or use `_MCFCRT_WrapThreadProcWithSehTop()` to wrap the callback.

* **Parameters**

    `_MCFCRT_NativeThreadProc pfnThreadProc`  
    The callback for the new thread to run.

    `void *pParam`  
    The parameter passed to `pfnThreadProc`.

    `bool bSuspended`  
    Whether the new thread should be suspended until explicitly resumed using `_MCFCRT_ResumeThread()`.

    `uintptr_t *restrict puThreadId`  
    The location where to store the thread ID of the new thread. This parameter can be `NULL`.  
    If this parameter is `NULL`, the thread ID is not returned.

* **Return Value**

    The return value is a handle to the new thread.  
    If the thread cannot be created, `NULL` is returned. To get extended error information, call `GetLastError()`.

* **Remarks**

    This function uses `RtlCreateUserThread()` to create a thread in the current process.  

#### `void _MCFCRT_CloseThread(_MCFCRT_ThreadHandle hThread);`

* **Description**

    This function closes the handle of a thread. A thread is deleted after it has terminated and all handles to it have been closed.

* **Parameters**

    `_MCFCRT_ThreadHandle hThread`  
    The handle to a thread to close.

* **Return Values**

    This function does not return a value.

#### _\(Since 2.x\)_ `unsigned long _MCFCRT_WrapThreadProcWithSehTop(_MCFCRT_WrappedThreadProc pfnThreadProc, void *pParam);`

* **Description**

    This function returns `(*pfnThreadProc)(pParam)`. The entire expression is placed in a special SEH handler in order to make SEH-based stack unwinding mechanism work. The SEH handler is uninstalled before control flow leaves this function normally.  
    On x86, this function additionally makes the `ESP` register align on 16-byte boundaries before the wrapped procedure is invoked. See description of `_MCFCRT_CreateNativeThread()` for the reason why this is necessary.

* **Parameters**

    `_MCFCRT_WrappedThreadProc pfnThreadProc`  
    The procedure to wrap.

    `void *pParam`  
    Parameter to the wrapped procedure.

* **Return Values**

    This function returns the return value of `(*pfnThreadProc)(pParam)`.

#### `void _MCFCRT_Sleep(uint64_t u64UntilFastMonoClock);`

* **Description**

    This function suspends the current thread until the specified time point is exceeded.

* **Parameters**

    `uint64_t u64UntilFastMonoClock`  
    If the value of the fast monotonic clock exceeds this value, the thread resumes execution.  
    This value shall be offset from the return value of `_MCFCRT_GetFastMonoClock()`.

* **Return value**

    This function does not return a value.

#### `bool _MCFCRT_AlertableSleep(uint64_t u64UntilFastMonoClock);`

* **Description**

    This function is similar to `_MCFCRT_Sleep()`, except that it sets the current thread in an alertable state and allows APC callbacks to be dispatched.

* **Parameters**

    `u64UntilFastMonoClock`  
    If the value of the fast monotonic clock exceeds this value, the thread resumes execution.  
    This value shall be offset from the return value of `_MCFCRT_GetFastMonoClock()`.

* **Return value**

    This function returns `true` if the thread is woken up by APC callbacks and `false` if the specified time point is exceeded.

#### `void _MCFCRT_AlertableSleepForever(void);`

* **Description**

    This function is an efficient version of `_MCFCRT_AlertableSleep()` that never times out hence does not return a value.

* **Parameters**

    This function does not take any parameters.

* **Return value**

    This function does not return a value.

#### `void _MCFCRT_YieldThread(void);`

* **Description**

    This function requests the kernel to reschedule the current thread.

* **Parameters**

    This function does not take any parameters.

* **Return value**

    This function does not return a value.

#### `long _MCFCRT_SuspendThread(_MCFCRT_ThreadHandle hThread);`

* **Description**

    This function increments the suspend count of the specified thread. If the counter is greater than zero, the thread is suspended.

* **Parameters**

    `_MCFCRT_ThreadHandle hThread`  
    A handle to the thread to suspend.

* **Return Value**

    The return value is the suspend count before the call.

#### `long _MCFCRT_ResumeThread(_MCFCRT_ThreadHandle hThread);`

* **Description**

    This function decrements the suspend count of the specified thread. If the counter is equal to zero, the thread is resumed.

* **Parameters**

    `_MCFCRT_ThreadHandle hThread`  
    A handle to the thread to resume.

* **Return Value**

    The return value is the suspend count before the call.

#### `bool _MCFCRT_WaitForThread(_MCFCRT_ThreadHandle hThread, uint64_t u64UntilFastMonoClock);`

* **Description**

    This function waits for another thread to terminate.

* **Parameters**

    `_MCFCRT_ThreadHandle hThread`  
    A handle to the thread to wait for.

    `u64UntilFastMonoClock`  
    If the value of the fast monotonic clock exceeds this value, the wait is abandoned and the current thread resumes execution with this function returning `false`.  
    This value shall be offset from the return value of `_MCFCRT_GetFastMonoClock()`.

* **Return value**

    This function returns `true` if the specified thread has terminated and `false` otherwise.

#### `void _MCFCRT_WaitForThreadForever(_MCFCRT_ThreadHandle hThread);`

* **Description**

    This function is an efficient version of `_MCFCRT_WaitForThread()` that never times out hence does not return a value.

* **Parameters**

    `_MCFCRT_ThreadHandle hThread`  
    A handle to the thread to wait for.

* **Return value**

    This function does not return a value.

#### `uintptr_t _MCFCRT_GetCurrentThreadId(void);`

* **Description**

    This function return the thread ID of the current thread.

* **Parameters**

    This function does not take any parameters.

* **Return value**

    The return value is the thread ID of the current thread.

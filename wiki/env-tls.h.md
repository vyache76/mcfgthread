# env/tls.h

## Macros

None.

## Types

#### `typedef unsigned long (*_MCFCRT_TlsConstructor)(intptr_t nContext, void *pStorage);`

* **Description**

    This is the type for the callback to initialize a TLS object.

#### `typedef void (*_MCFCRT_TlsDestructor)(intptr_t nContext, void *pStorage);`

* **Description**

    This is the type for the callback to destroy a TLS object.

#### `typedef struct __MCFCRT_tagTlsKeyHandle *_MCFCRT_TlsKeyHandle;`

* **Description**

    This is an opaque pointer type playing a role as the handle of a TLS key.  

#### `typedef void (*_MCFCRT_AtThreadExitCallback)(intptr_t nContext);`

* **Description**

    This is the type for callbacks that can be registered to be invoked upon the current thread's exit.

## Functions

#### `_MCFCRT_TlsKeyHandle _MCFCRT_TlsAllocKey(size_t uSize, _MCFCRT_TlsConstructor pfnConstructor, _MCFCRT_TlsDestructor pfnDestructor, intptr_t nContext);`

* **Description**

    This function allocates a TLS key and returns its handle. TLS keys are used to access thread local objects.

* **Parameters**

    `size_t uSize`  
    The size of each thread local object that this TLS key refers.  
    TLS objects are aligned to `alignof(max_align_t)`. Over-aligned types are not supported.

    `_MCFCRT_TlsConstructor pfnConstructor`  
    The callback invoked to initialize a thread local object. This parameter can be `NULL`.  
    If `NULL` is provided, the thread local object is zero-initialized.

    `_MCFCRT_TlsDestructor pfnDestructor`  
    The callback invoked to destroy a thread local object when a thread exits. This parameter can be `NULL`.  
    The destructor is called on each object that has been allocated successfully. In other words, it is guaranteed that the `pStorage` parameter of the callback will not be null.  
    If `NULL` is provided, no cleanup is done.

    `intptr_t nContext`  
    Common user-defined parameter for both callbacks.  

* **Return value**

    This function returns a handle to the newly allocated TLS key.  
    If the allocation fails, `NULL` is returned. To get extended error information, call `GetLastError()`.

* **Remarks**

    The maximum number of TLS keys is only bounded by available memory.

#### `void _MCFCRT_TlsFreeKey(_MCFCRT_TlsKeyHandle hTlsKey);`

* **Description**

    This function frees the TLS key allocated by `_MCFCRT_TlsAllocKey()`.  
    This function does not destroy thread local objects referred by this TLS key. A thread local object is destroyed only when its owning thread exits normally (that is, not if its owning thread is terminated because a thread calls `TerminateThread()` with its handle or another thread calls `ExitProcess()` whatsoever).

* **Parameters**

    `_MCFCRT_TlsKeyHandle hTlsKey`  
    The handle to a TLS key.

* **Return Value**

    This function does not return a value.

#### `size_t _MCFCRT_TlsGetSize(_MCFCRT_TlsKeyHandle hTlsKey);`
#### `_MCFCRT_TlsConstructor _MCFCRT_TlsGetConstructor(_MCFCRT_TlsKeyHandle hTlsKey);`
#### `_MCFCRT_TlsDestructor _MCFCRT_TlsGetDestructor(_MCFCRT_TlsKeyHandle hTlsKey);`
#### `intptr_t _MCFCRT_TlsGetContext(_MCFCRT_TlsKeyHandle hTlsKey);`

* **Description**

    These functions return the parameters that were used to create the specified TLS key.

* **Parameters**

    `_MCFCRT_TlsKeyHandle hTlsKey`  
    The handle to a TLS key.

* **Return Value**

    These functions return the parameters passed to `_MCFCRT_TlsAllocKey()`.

* **Remarks**

    These functions provide no error checking.  
    If the TLS key isn't allocated by `_MCFCRT_TlsAllocKey()` or has already been freed by `_MCFCRT_TlsFreeKey()`, the behavior is undefined.

#### `bool _MCFCRT_TlsGet(_MCFCRT_TlsKeyHandle hTlsKey, void **restrict ppStorage);`

* **Description**

    This function looks for the thread local object that is referred by `hTlsKey` and owned by the current thread.  
    If such an object has been created, a pointer to the object is returned into `*ppStorage`.  
    Otherwise, `NULL` is returned into `*ppStorage`.

* **Parameters**

    `_MCFCRT_TlsKeyHandle hTlsKey`  
    The handle to a TLS key.

    `void **restrict ppStorage`  
    The location where a pointer to the thread local object will be stored.

* **Return Value**

    This function returns `true` on success and `false` on failure. To get extended error information, call `GetLastError()`.

* **Remarks**

    This function never fails as long as `hTlsKey` is valid.

#### `bool _MCFCRT_TlsRequire(_MCFCRT_TlsKeyHandle hTlsKey, void **restrict ppStorage);`

* **Description**

    This function looks for the thread local object that is referred by `hTlsKey` and owned by the current thread.  
    If such an object has been created, a pointer to the object is stored into `*ppStorage`.  
    Otherwise, the storage for a new object is allocated and bitwise filled with zeroes. If the `pfnConstructor` parameter passed to `_MCFCRT_TlsAllocKey()` wasn't `NULL`, the specified callback is invoked to initialize the object. Then:  
* If the callback returns zero, the initialization is considered to have succeeded. A pointer to the newly created object is stored into `*ppStorage`, and this function returns `true`.  
* If the callback returns a non-zero value, the initialization is considered to have failed. The storage for the new object is immediately deallocated, `SetLastError()` is called with that return value, and this function returns `false`.  

* **Parameters**

    `_MCFCRT_TlsKeyHandle hTlsKey`  
    The handle to a TLS key.

    `void **restrict ppStorage`  
    The location where a pointer to the thread local object will be stored.

* **Return Value**

    This function returns `true` on success and `false` on failure. To get extended error information, call `GetLastError()`.

#### `bool _MCFCRT_AtThreadExit(_MCFCRT_AtThreadExitCallback pfnProc, intptr_t nContext);`

* **Description**

    This function registers a callback to be invoked upon the current thread's exit.  
    The callbacks are invoked in a first in, last out order.

* **Parameters**

    `_MCFCRT_AtThreadExitCallback pfnProc`  
    The callback to be invoked.  

    `intptr_t nContext`  
    Parameter for the callback.

* **Return Value**

    This function returns `true` on success and `false` on failure. To get extended error information, call `GetLastError()`.

* **Remarks**

    It is possible to call this function in other thread-exit callbacks. Callbacks registered in such scenarios are guaranteed to be invoked in a first in, last out order, but could otherwise interleave with existent callbacks.

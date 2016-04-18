// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_ENV_THREAD_ENV_H_
#define __MCFCRT_ENV_THREAD_ENV_H_

#include "_crtdef.h"

_MCFCRT_EXTERN_C_BEGIN

extern bool __MCFCRT_ThreadEnvInit(void) _MCFCRT_NOEXCEPT;
extern void __MCFCRT_ThreadEnvUninit(void) _MCFCRT_NOEXCEPT;

extern void __MCFCRT_TlsCleanup() _MCFCRT_NOEXCEPT;

// The constructor shall return 0 upon success. Otherwise, see comments above _MCFCRT_TlsRequire().
typedef unsigned long (*_MCFCRT_TlsConstructor)(_MCFCRT_STD intptr_t __nContext, void *__pStorage);
typedef void (*_MCFCRT_TlsDestructor)(_MCFCRT_STD intptr_t __nContext, void *__pStorage);

// _MCFCRT_TlsAllocKey() returns `nullptr` upon failure.
extern void *_MCFCRT_TlsAllocKey(_MCFCRT_STD size_t __uSize, _MCFCRT_TlsConstructor __pfnConstructor, _MCFCRT_TlsDestructor __pfnDestructor, _MCFCRT_STD intptr_t __nContext) _MCFCRT_NOEXCEPT;
extern bool _MCFCRT_TlsFreeKey(void *__pTlsKey) _MCFCRT_NOEXCEPT;

extern _MCFCRT_STD size_t _MCFCRT_TlsGetSize(void *__pTlsKey) _MCFCRT_NOEXCEPT;
extern _MCFCRT_TlsConstructor _MCFCRT_TlsGetConstructor(void *__pTlsKey) _MCFCRT_NOEXCEPT;
extern _MCFCRT_TlsDestructor _MCFCRT_TlsGetDestructor(void *__pTlsKey) _MCFCRT_NOEXCEPT;
extern _MCFCRT_STD intptr_t _MCFCRT_TlsGetContext(void *__pTlsKey) _MCFCRT_NOEXCEPT;

// _MCFCRT_TlsGet() never fails, provided __pTlsKey is a valid key.
// If the storage for the current thread has not been initialized, a null pointer is returned into *__ppStorage.
extern bool _MCFCRT_TlsGet(void *__pTlsKey, void **restrict __ppStorage) _MCFCRT_NOEXCEPT;
// _MCFCRT_TlsRequire() is identical to _MCFCRT_TlsGet() if the storage for the current thread has already been initialized.
// If it is not, the storage is allocated and the constructor is called, and if the constructor returns 0, a pointer to the initialized storage is returned into *__ppStorage.
// If memory allocation fails, `false` is returned when GetLastError() returns ERROR_NOT_ENOUGH_MEMORY.
// If the constructor returns a non-zero value, the storage is deallocated immediately and `false` is returned when GetLastError() returns that non-zero value.
extern bool _MCFCRT_TlsRequire(void *__pTlsKey, void **restrict __ppStorage) _MCFCRT_NOEXCEPT;

typedef void (*_MCFCRT_AtThreadExitCallback)(_MCFCRT_STD intptr_t __nContext);

extern bool _MCFCRT_AtThreadExit(_MCFCRT_AtThreadExitCallback __pfnProc, _MCFCRT_STD intptr_t __nContext) _MCFCRT_NOEXCEPT;

_MCFCRT_EXTERN_C_END

#endif

// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2017, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_ENV_LIBCXXTHREAD_H_
#define __MCFCRT_ENV_LIBCXXTHREAD_H_

// Compatibility layer for LLVM libc++.

#include "_crtdef.h"
#include "_mopthread.h"
#include "tls.h"
#include "once_flag.h"
#include "mutex.h"
#include "condition_variable.h"
#include "clocks.h"
#include "../ext/assert.h"
#include "../ext/expect.h"
#include <time.h> // struct timespec
#include <errno.h>

#ifndef __MCFCRT_LIBCXXTHREAD_INLINE_OR_EXTERN
#	define __MCFCRT_LIBCXXTHREAD_INLINE_OR_EXTERN     __attribute__((__gnu_inline__)) extern inline
#endif

_MCFCRT_EXTERN_C_BEGIN

_MCFCRT_EXTERN_C_END

#endif

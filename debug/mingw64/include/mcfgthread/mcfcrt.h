// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2017, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_MCFCRT_H_
#define __MCFCRT_MCFCRT_H_

#include "env/_crtdef.h"

#include "env/avl_tree.h"
#include "env/bail.h"
#include "env/clocks.h"
#include "env/condition_variable.h"
#include "env/xassert.h"
#include "env/crt_module.h"
#include "env/expect.h"
#include "env/heap.h"
#include "env/inline_mem.h"
#include "env/mutex.h"
#include "env/once_flag.h"
#include "env/pp.h"
#include "env/thread.h"
#include "env/tls.h"

#include "ext/itow.h"
#include "ext/wcpcpy.h"

_MCFCRT_EXTERN_C_BEGIN

extern bool __MCFCRT_InitRecursive(void) _MCFCRT_NOEXCEPT;
extern void __MCFCRT_UninitRecursive(void) _MCFCRT_NOEXCEPT;

_MCFCRT_EXTERN_C_END

#endif

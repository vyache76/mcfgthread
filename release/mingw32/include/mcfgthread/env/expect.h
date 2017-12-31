// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2018, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_ENV_EXPECT_H_
#define __MCFCRT_ENV_EXPECT_H_

#include "_crtdef.h"

#define _MCFCRT_EXPECT(__x_)        (__builtin_expect((__x_) ? true : false, true ))
#define _MCFCRT_EXPECT_NOT(__x_)    (__builtin_expect((__x_) ? true : false, false))

#endif

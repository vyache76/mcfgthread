// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "crt_module.h"

bool _MCFCRT_AtCrtModuleExit(_MCFCRT_AtModuleExitCallback pfnProc, intptr_t nContext){
	return _MCFCRT_AtModuleExit(pfnProc, nContext);
}

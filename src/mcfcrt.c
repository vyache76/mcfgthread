// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2017, LH_Mouse. All wrongs reserved.

#include "mcfcrt.h"
#include "env/mcfwin.h"
#include "pre/module.h"
#include "env/tls.h"
#include "env/crt_module.h"

bool __MCFCRT_Init(void){
	if(!__MCFCRT_ModuleInit()){
		return false;
	}
	if(!__MCFCRT_TlsInit()){
		__MCFCRT_ModuleUninit();
		return false;
	}
	return true;
}
void __MCFCRT_Uninit(void){
	__MCFCRT_TlsUninit();
	__MCFCRT_ModuleUninit();
	__MCFCRT_DiscardCrtModuleQuickExitCallbacks();
}

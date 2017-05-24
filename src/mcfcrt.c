// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2017, LH_Mouse. All wrongs reserved.

#include "mcfcrt.h"
#include "env/tls.h"

static ptrdiff_t g_nCounter = 0;

bool __MCFCRT_InitRecursive(void){
	ptrdiff_t nCounter = g_nCounter;
	if(nCounter == 0){
		if(!__MCFCRT_TlsInit()){
			return false;
		}
		// Add more initialization...
	}
	++nCounter;
	g_nCounter = nCounter;
	return true;
}
void __MCFCRT_UninitRecursive(void){
	ptrdiff_t nCounter = g_nCounter;
	_MCFCRT_ASSERT(nCounter > 0);
	--nCounter;
	g_nCounter = nCounter;
	if(nCounter == 0){
		// Add more uninitialization...
		__MCFCRT_TlsUninit();
	}
}

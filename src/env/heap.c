// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2018, LH_Mouse. All wrongs reserved.

#include "heap.h"
#include "mcfwin.h"

void *__MCFCRT_HeapAlloc(size_t uNewSize, bool bFillsWithZero, const void *pRetAddrOuter){
	(void)pRetAddrOuter;

	return HeapAlloc(GetProcessHeap(), bFillsWithZero ? HEAP_ZERO_MEMORY : 0, uNewSize);
}
void *__MCFCRT_HeapRealloc(void *pOldBlock, size_t uNewSize, bool bFillsWithZero, const void *pRetAddrOuter){
	(void)pRetAddrOuter;

	return HeapReAlloc(GetProcessHeap(), bFillsWithZero ? HEAP_ZERO_MEMORY : 0, pOldBlock, uNewSize);
}
void __MCFCRT_HeapFree(void *pOldBlock, const void *pRetAddrOuter){
	(void)pRetAddrOuter;

	HeapFree(GetProcessHeap(), 0, pOldBlock);
}


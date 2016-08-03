// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "clocks.h"
#include "mcfwin.h"
#include "bail.h"

static uint64_t GetTimeZoneOffsetInMillisecondsOnce(void){
	static uint64_t *volatile s_pu64Inited;
	static uint64_t           s_u64Value;

	uint64_t *pInited = __atomic_load_n(&s_pu64Inited, __ATOMIC_CONSUME);
	if(!pInited){
		pInited = &s_u64Value;

		TIME_ZONE_INFORMATION vTzInfo;
		if(GetTimeZoneInformation(&vTzInfo) == TIME_ZONE_ID_INVALID){
			_MCFCRT_Bail(L"GetTimeZoneInformation() failed.");
		}
		*pInited = (uint64_t)(vTzInfo.Bias * -60000ll);

		__atomic_store_n(&s_pu64Inited, pInited, __ATOMIC_RELEASE);
	}
	return *pInited;
}
static double QueryPerformanceFrequencyReciprocalOnce(void){
	static double *volatile s_plfInited;
	static double           s_ulfValue;

	double *pInited = __atomic_load_n(&s_plfInited, __ATOMIC_CONSUME);
	if(!pInited){
		pInited = &s_ulfValue;

		LARGE_INTEGER liFreq;
		if(!QueryPerformanceFrequency(&liFreq)){
			_MCFCRT_Bail(L"QueryPerformanceFrequency() failed.");
		}
		*pInited = 1000.0 / (double)liFreq.QuadPart;

		__atomic_store_n(&s_plfInited, pInited, __ATOMIC_RELEASE);
	}
	return *pInited;
}

uint64_t _MCFCRT_GetUtcClock(void){
	union {
		FILETIME ft;
		LARGE_INTEGER li;
	} unUtc;
	GetSystemTimeAsFileTime(&unUtc.ft);
	// 0x019DB1DED53E8000 = duration since 1601-01-01 until 1970-01-01 in nanoseconds.
	return (uint64_t)(((double)unUtc.li.QuadPart - 0x019DB1DED53E8000ll) / 10000.0);
}
uint64_t _MCFCRT_GetLocalClock(void){
	return _MCFCRT_GetLocalClockFromUtc(_MCFCRT_GetUtcClock());
}

uint64_t _MCFCRT_GetUtcClockFromLocal(uint64_t u64LocalClock){
	return u64LocalClock - GetTimeZoneOffsetInMillisecondsOnce();
}
uint64_t _MCFCRT_GetLocalClockFromUtc(uint64_t u64UtcClock){
	return u64UtcClock + GetTimeZoneOffsetInMillisecondsOnce();
}

uint64_t _MCFCRT_GetFastMonoClock(void){
	return GetTickCount64();
}
double _MCFCRT_GetHiResMonoClock(void){
	LARGE_INTEGER liCounter;
	if(!QueryPerformanceCounter(&liCounter)){
		_MCFCRT_Bail(L"QueryPerformanceCounter() failed.");
	}
	return (double)liCounter.QuadPart * QueryPerformanceFrequencyReciprocalOnce();
}

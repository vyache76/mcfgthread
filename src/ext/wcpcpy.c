// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2018, LH_Mouse. All wrongs reserved.

#include "wcpcpy.h"
#include "../env/expect.h"
#include "../env/xassert.h"

#ifdef __SSE2__

#include <intrin.h>
#include <emmintrin.h>

wchar_t *_MCFCRT_wcpcpy(wchar_t *restrict dst, const wchar_t *restrict src){
	register wchar_t *wp = dst;
	register const wchar_t *rp = src;
	// Align `rp` to word boundaries, only by then can we read one word at a time.
	// Since memory access applies to pages, as long as we read an aligned word we can never access across pages,
	// which leads to a segment fault if the current page is readable but the next page is not.
	while(((uintptr_t)rp & 31) != 0){
#define CPY_GEN()	\
		{	\
			const wchar_t rc = *rp;	\
			*wp = rc;	\
			if(rc == 0){	\
				return wp;	\
			}	\
			++rp;	\
			++wp;	\
		}
		CPY_GEN()
	}
#define SSE2_CPY(save_, load_)	\
	{	\
		const __m128i xz = _mm_setzero_si128();	\
		for(;;){	\
			const __m128i xw0 = (load_)((const __m128i *)rp);	\
			const __m128i xw1 = (load_)((const __m128i *)rp + 1);	\
			__m128i xt = _mm_packs_epi16(_mm_cmpeq_epi16(xw0, xz), _mm_cmpeq_epi16(xw1, xz));	\
			uint32_t mask = (uint32_t)_mm_movemask_epi8(xt);	\
			if(_MCFCRT_EXPECT_NOT(mask != 0)){	\
				const unsigned tz = (unsigned)__builtin_ctz(mask);	\
				__movsw((void *)wp, (const void *)rp, tz);	\
				wp += tz;	\
				*wp = 0;	\
				return wp;	\
			}	\
			(save_)((__m128i *)wp, xw0);	\
			(save_)((__m128i *)wp + 1, xw1);	\
			rp += 16;	\
			wp += 16;	\
		}	\
	}
	if(((uintptr_t)wp & 15) == 0){
		SSE2_CPY(_mm_store_si128, _mm_load_si128)
	} else {
		SSE2_CPY(_mm_storeu_si128, _mm_load_si128)
	}
}
wchar_t *_MCFCRT_wcppcpy(wchar_t *dst, wchar_t *end, const wchar_t *restrict src){
	_MCFCRT_ASSERT(dst + 1 <= end);
	register wchar_t *wp = dst;
	wchar_t *const wend = end - 1;
	register const wchar_t *rp = src;
	// Align `rp` to word boundaries, only by then can we read one word at a time.
	// Since memory access applies to pages, as long as we read an aligned word we can never access across pages,
	// which leads to a segment fault if the current page is readable but the next page is not.
	while(((uintptr_t)rp & 31) != 0){
#define PCPY_GEN()	\
		{	\
			if(wp == wend){	\
				*wp = 0;	\
				return wp;	\
			}	\
			const wchar_t rc = *rp;	\
			*wp = rc;	\
			if(rc == 0){	\
				return wp;	\
			}	\
			++rp;	\
			++wp;	\
		}
		PCPY_GEN()
	}
	if((size_t)(wend - wp) >= 64){
#define SSE2_PCPY(save_, load_)	\
		{	\
			const __m128i xz = _mm_setzero_si128();	\
			do {	\
				const __m128i xw0 = (load_)((const __m128i *)rp);	\
				const __m128i xw1 = (load_)((const __m128i *)rp + 1);	\
				__m128i xt = _mm_packs_epi16(_mm_cmpeq_epi16(xw0, xz), _mm_cmpeq_epi16(xw1, xz));	\
				uint32_t mask = (uint32_t)_mm_movemask_epi8(xt);	\
				if(_MCFCRT_EXPECT_NOT(mask != 0)){	\
					const unsigned tz = (unsigned)__builtin_ctz(mask);	\
					__movsw((void *)wp, (const void *)rp, tz);	\
					wp += tz;	\
					*wp = 0;	\
					return wp;	\
				}	\
				(save_)((__m128i *)wp, xw0);	\
				(save_)((__m128i *)wp + 1, xw1);	\
				rp += 16;	\
				wp += 16;	\
			} while((size_t)(wend - wp) >= 16);	\
		}
		if(((uintptr_t)wp & 15) == 0){
			SSE2_PCPY(_mm_store_si128, _mm_load_si128)
		} else {
			SSE2_PCPY(_mm_storeu_si128, _mm_load_si128)
		}
	}
	for(;;){
		PCPY_GEN()
	}
}

#else // __SSE2__

#ifdef _WIN64
#  define MASK     0x0001000100010001ull
#else
#  define MASK     0x00010001ul
#endif

wchar_t *_MCFCRT_wcpcpy(wchar_t *restrict dst, const wchar_t *restrict src){
	register const wchar_t *rp = src;
	register wchar_t *wp = dst;

	if(((uintptr_t)rp & 1) != 0){
		for(;;){
			if((*wp = *(rp++)) == 0){
				return wp;
			}
			++wp;
		}
	}

	// Align rp to word boundaries, only by then can we read one word at a time.
	// Since memory access applies to pages, as long as we read an aligned word we can never access across pages,
	// which leads to a segment fault if the current page is readable but the next page is not.
	while(((uintptr_t)rp & (sizeof(uintptr_t) - 1)) != 0){
		if((*wp = *(rp++)) == 0){
			return wp;
		}
		++wp;
	}

	for(;;){

#define UNROLLED(idx_)	\
		{	\
			register uintptr_t wrd = ((const uintptr_t *)rp)[(idx_)];	\
			if(((wrd - MASK) & ~wrd & (MASK << 15)) != 0){	\
				wp += (idx_) * sizeof(uintptr_t) / sizeof(wchar_t);	\
				for(size_t i = 0; i < sizeof(uintptr_t) / sizeof(wchar_t) - 1; ++i){	\
					if((*wp = (wchar_t)(uint16_t)(wrd & 0xFFFF)) == 0){	\
						return wp;	\
					}	\
					++wp;	\
					wrd >>= 16;	\
				}	\
				*wp = 0;	\
				return wp;	\
			}	\
			((uintptr_t *)wp)[(idx_)] = wrd;	\
		}

		UNROLLED(0)
		UNROLLED(1)
		UNROLLED(2)
		UNROLLED(3)
		UNROLLED(4)
		UNROLLED(5)
		UNROLLED(6)
		UNROLLED(7)

		rp += 8 * sizeof(uintptr_t) / sizeof(wchar_t);
		wp += 8 * sizeof(uintptr_t) / sizeof(wchar_t);
	}
}
wchar_t *_MCFCRT_wcppcpy(wchar_t *dst, wchar_t *end, const wchar_t *restrict src){
	register const wchar_t *rp = src;
	register wchar_t *wp = dst;

	_MCFCRT_ASSERT_MSG(wp < end, L"The destination buffer must be at least one characterlarge to store the null terminator.");

	if(((uintptr_t)rp & 1) != 0){
		for(;;){
			if((*wp = *(rp++)) == 0){
				return wp;
			}
			++wp;
		}
	}

	// Align rp to word boundaries, only by then can we read one word at a time.
	// Since memory access applies to pages, as long as we read an aligned word we can never access across pages,
	// which leads to a segment fault if the current page is readable but the next page is not.
	while(((uintptr_t)rp & (sizeof(uintptr_t) - 1)) != 0){
		if(wp >= end - 1){
			*wp = 0;
			return wp;
		}
		if((*wp = *(rp++)) == 0){
			return wp;
		}
		++wp;
	}

	for(;;){

#define UNROLLED_P(idx_)	\
		{	\
			register uintptr_t wrd = ((const uintptr_t *)rp)[(idx_)];	\
			if((size_t)(end - 1 - wp) < ((idx_) + 1) * sizeof(uintptr_t) / sizeof(wchar_t)){	\
				wp += (idx_) * sizeof(uintptr_t) / sizeof(wchar_t);	\
				for(size_t i = 0; i < sizeof(uintptr_t) / sizeof(wchar_t) - 1; ++i){	\
					if(wp >= end - 1){	\
						*wp = 0;	\
						return wp;	\
					}	\
					if((*wp = (wchar_t)(uint16_t)(wrd & 0xFFFF)) == 0){	\
						return wp;	\
					}	\
					++wp;	\
					wrd >>= 16;	\
				}	\
				*wp = 0;	\
				return wp;	\
			}	\
			if(((wrd - MASK) & ~wrd & (MASK << 15)) != 0){	\
				wp += (idx_) * sizeof(uintptr_t) / sizeof(wchar_t);	\
				for(size_t i = 0; i < sizeof(uintptr_t) / sizeof(wchar_t) - 1; ++i){	\
					if((*wp = (wchar_t)(uint16_t)(wrd & 0xFFFF)) == 0){	\
						return wp;	\
					}	\
					++wp;	\
					wrd >>= 16;	\
				}	\
				*wp = 0;	\
				return wp;	\
			}	\
			((uintptr_t *)wp)[(idx_)] = wrd;	\
		}

		UNROLLED_P(0)
		UNROLLED_P(1)
		UNROLLED_P(2)
		UNROLLED_P(3)
		UNROLLED_P(4)
		UNROLLED_P(5)
		UNROLLED_P(6)
		UNROLLED_P(7)

		rp += 8 * sizeof(uintptr_t) / sizeof(wchar_t);
		wp += 8 * sizeof(uintptr_t) / sizeof(wchar_t);
	}
}

#endif // __SSE2__

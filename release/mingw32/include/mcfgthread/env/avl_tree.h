// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2017, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_ENV_AVL_TREE_H_
#define __MCFCRT_ENV_AVL_TREE_H_

#include "_crtdef.h"

_MCFCRT_EXTERN_C_BEGIN

typedef struct __MCFCRT_tagAvlNodeHeader {
	struct __MCFCRT_tagAvlNodeHeader *__pParent;
	struct __MCFCRT_tagAvlNodeHeader **__ppRefl;
	struct __MCFCRT_tagAvlNodeHeader *__pLeft;
	struct __MCFCRT_tagAvlNodeHeader *__pRight;
	_MCFCRT_STD size_t __uHeight;
	struct __MCFCRT_tagAvlNodeHeader *__pPrev;
	struct __MCFCRT_tagAvlNodeHeader *__pNext;
} _MCFCRT_AvlNodeHeader, *_MCFCRT_AvlRoot;

extern _MCFCRT_AvlNodeHeader *_MCFCRT_AvlPrev(const _MCFCRT_AvlNodeHeader *__pNode) _MCFCRT_NOEXCEPT;
extern _MCFCRT_AvlNodeHeader *_MCFCRT_AvlNext(const _MCFCRT_AvlNodeHeader *__pNode) _MCFCRT_NOEXCEPT;

extern void _MCFCRT_AvlInternalAttach(_MCFCRT_AvlNodeHeader *__pNode, _MCFCRT_AvlNodeHeader *__pParent, _MCFCRT_AvlNodeHeader **__ppRefl) _MCFCRT_NOEXCEPT;
extern void _MCFCRT_AvlInternalDetach(const _MCFCRT_AvlNodeHeader *__pNode) _MCFCRT_NOEXCEPT;

extern _MCFCRT_AvlNodeHeader *_MCFCRT_AvlFront(const _MCFCRT_AvlRoot *__ppRoot) _MCFCRT_NOEXCEPT;
extern _MCFCRT_AvlNodeHeader *_MCFCRT_AvlBack(const _MCFCRT_AvlRoot *__ppRoot) _MCFCRT_NOEXCEPT;

extern void _MCFCRT_AvlSwap(_MCFCRT_AvlRoot *__ppRoot1, _MCFCRT_AvlRoot *__ppRoot2) _MCFCRT_NOEXCEPT;

// These comparators shall return a negative, zero or positive value if the left operand is less than, equal to or greater than the right operand, respectively.
typedef int (*_MCFCRT_AvlComparatorNodes)(const _MCFCRT_AvlNodeHeader *, const _MCFCRT_AvlNodeHeader *);
typedef int (*_MCFCRT_AvlComparatorNodeOther)(const _MCFCRT_AvlNodeHeader *, _MCFCRT_STD intptr_t);

// Insertion is optimized if `__pHint` is non-null and `__pNode` is inserted to an adjacent position to either side of `__pHint`.
// Such behavior is compatible with both C++98 and C++11.
extern void _MCFCRT_AvlAttachWithHint(_MCFCRT_AvlRoot *__ppRoot, const _MCFCRT_AvlNodeHeader *__pHint, _MCFCRT_AvlNodeHeader *__pNode, _MCFCRT_AvlComparatorNodes __pfnComparator) _MCFCRT_NOEXCEPT;
extern void _MCFCRT_AvlAttach(_MCFCRT_AvlRoot *__ppRoot, _MCFCRT_AvlNodeHeader *__pNode, _MCFCRT_AvlComparatorNodes __pfnComparator) _MCFCRT_NOEXCEPT;
extern void _MCFCRT_AvlDetach(const _MCFCRT_AvlNodeHeader *__pNode) _MCFCRT_NOEXCEPT;

extern _MCFCRT_AvlNodeHeader *_MCFCRT_AvlGetLowerBound(const _MCFCRT_AvlRoot *__ppRoot,
	_MCFCRT_STD intptr_t __nOther, _MCFCRT_AvlComparatorNodeOther __pfnComparatorNodeOther) _MCFCRT_NOEXCEPT;
extern _MCFCRT_AvlNodeHeader *_MCFCRT_AvlGetUpperBound(const _MCFCRT_AvlRoot *__ppRoot,
	_MCFCRT_STD intptr_t __nOther, _MCFCRT_AvlComparatorNodeOther __pfnComparatorNodeOther) _MCFCRT_NOEXCEPT;
extern _MCFCRT_AvlNodeHeader *_MCFCRT_AvlFind(const _MCFCRT_AvlRoot *__ppRoot,
	_MCFCRT_STD intptr_t __nOther, _MCFCRT_AvlComparatorNodeOther __pfnComparatorNodeOther) _MCFCRT_NOEXCEPT;
extern void _MCFCRT_AvlGetEqualRange(_MCFCRT_AvlNodeHeader **__ppLower, _MCFCRT_AvlNodeHeader **__ppUpper, const _MCFCRT_AvlRoot *__ppRoot,
	_MCFCRT_STD intptr_t __nOther, _MCFCRT_AvlComparatorNodeOther __pfnComparatorNodeOther) _MCFCRT_NOEXCEPT;

_MCFCRT_EXTERN_C_END

#ifndef __MCFCRT_AVL_TREE_INLINE_OR_EXTERN
#	define __MCFCRT_AVL_TREE_INLINE_OR_EXTERN     __attribute__((__gnu_inline__)) extern inline
#endif
#include "_avl_tree_inl.h"

#endif

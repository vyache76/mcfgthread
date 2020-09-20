# env/heap.h

## Macros

None.

## Types

None.

## Functions

#### `void *_MCFCRT_malloc(size_t uSize);`
#### `void *_MCFCRT_realloc(void *pBlock, size_t uSize);`
#### `void *_MCFCRT_calloc(size_t uCount, size_t uBlockSize);`
#### `void _MCFCRT_free(void *pBlock);`

* **Description**

    These functions have the same semantics with the ones that have the `_MCFCRT_` prefix removed from their names in the C standard library.

* **Parameters**

    See [C memory management library](http://en.cppreference.com/w/c/memory).

* **Return Value**

    See [C memory management library](http://en.cppreference.com/w/c/memory).

* **Remarks**

    These functions use Windows [heap functions](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366711.aspx) directly and are not wrappers for C library functions from `MSVCRT.DLL`. Hence, for example, passing the value returned from `_MCFCRT_malloc()` to `free()` results in undefined behavior.

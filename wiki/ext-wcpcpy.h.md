# ext/wcpcpy.h

## Macros

None.

## Types

None.

## Functions

#### `wchar_t *_MCFCRT_wcpcpy(wchar_t *restrict dst, const wchar_t *restrict src);`

* **Description**

    This function copies the wide string pointed by `src` to the buffer pointed by `dst`.  
    If the two strings overlap, the behavior is undefined.  
    The copy operation stops after a null terminator is encountered in the source string, which is also copied.  

* **Parameters**

    `wchar_t *restrict dst`  
    The location where the destination string will be written to.  

    `const wchar_t *restrict src`  
    The location where the source string resides.

* **Return value**

    This function returns a pointer to the (newly created) null terminator of the destination string.

* **Remarks**

    This function is equivalent to the `wcpcpy()` function in POSIX-2008.

#### `wchar_t *_MCFCRT_wcppcpy(wchar_t *dst, wchar_t *end, const wchar_t *restrict src);`

This function copies the wide string pointed by `src` to the buffer pointed by `dst`.  
If the two strings overlap, the behavior is undefined.  

The copy operation stops in either case:  
1. A null terminator is encountered in the source string, which is also copied.  
2. Copying a character would overwrite `end[-1]`, in which case a null terminator is written to `end[-1]`.  

If the expression `dst < end` is not `true` before calling this function, the behavior is undefined.  
In particular, if `dst` equals `end - 1` before calling this function, no character will be copied and the function writes a null terminator to `*dst` and returns `dst`.

* **Parameters**

    `wchar_t *dst`  
    The location where the destination string will be written to.  

    `wchar_t *end`  
    The location where the destination buffer ends. No character will be written beyond `end`.  

    `const wchar_t *restrict src`  
    The location where the source string resides.

* **Return value**

    This function returns a pointer to the (newly created) null terminator of the destination string.

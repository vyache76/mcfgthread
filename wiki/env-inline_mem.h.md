# env/inline_mem.h

## Macros

None.

## Types

None.

## Functions

#### `static inline void *_MCFCRT_inline_mempcpy_fwd(void *s1, const void *s2, size_t n);`

* **Description**

    This function copies a region of memory to another location. The region is always copied from lower addresses to higher addresses.

* **Parameters**

    `void *s1`  
    The location where bytes will be written to.  

    `const void *s2`  
    The location where bytes will be read from.  

    `size_t n`  
    Number of bytes to copy.  

* **Return Value**

    This function returns a pointer to the next byte to the last one copied. That is, `(void *)((char *)s1 + n)`.

#### `static inline void *_MCFCRT_inline_mempset_fwd(void *s, int c, size_t n);`

* **Description**

    This function fills a region of memory with the specified byte. The region is always filled from lower addresses to higher addresses.

* **Parameters**

    `void *s`  
    The location where bytes will be written to.  

    `int c`  
    The byte to write. High-order bits that do not fit into a byte are ignored.  

    `size_t n`  
    Number of bytes to fill.  

* **Return Value**

    This function returns a pointer to the next byte to the last one filled. That is, `(void *)((char *)s + n)`.

# ext/itow.h

## Macros

None.

## Types

None.

## Functions

#### `wchar_t *_MCFCRT_itow0d(wchar_t *buffer,  intptr_t value, unsigned min_digits);`
#### `wchar_t *_MCFCRT_itow0u(wchar_t *buffer, uintptr_t value, unsigned min_digits);`
#### `wchar_t *_MCFCRT_itow0x(wchar_t *buffer, uintptr_t value, unsigned min_digits);`
#### `wchar_t *_MCFCRT_itow0X(wchar_t *buffer, uintptr_t value, unsigned min_digits);`

* **Description**

    These functions convert integers to wide strings. The converted strings are NOT null-terminated.  

    `_MCFCRT_itow0d()` converts a signed integer to its decimal form, which is prefixed with a minus sign if the integer is less than zero.  

    `_MCFCRT_itow0u()` converts an unsigned integer to its decimal form.  

    `_MCFCRT_itow0x()` converts an unsigned integer to its hexadecimal form, with `a-f` in lowercase, if any.  

    `_MCFCRT_itow0X()` converts an unsigned integer to its hexadecimal form, with `A-F` in uppercase, if any.

* **Parameters**

    `wchar_t *buffer`  
    The location where the converted string will be written to.  

    `intptr_t value` or `uintptr_t value`  
    The integer to convert.  

    `unsigned min_digits`  
    Minimum number of digits to output, excluding the minus sign if the integer is signed and less than zero.  
    If the converted string has fewer digits than this number it is prefixed with zeroes.

* **Return Value**

    These functions return a pointer to the next character of the last one of the converted string.  

* **Remarks**

    None of these functions provide bound checking. The user must ensure that the output buffer is large enough to store the converted string.
    Overwriting the location that a returned pointer points to with `L'\0'` results in a null-terminated string.

#### `wchar_t *_MCFCRT_itow_d(wchar_t *buffer,  intptr_t value);`
#### `wchar_t *_MCFCRT_itow_u(wchar_t *buffer, uintptr_t value);`
#### `wchar_t *_MCFCRT_itow_x(wchar_t *buffer, uintptr_t value);`
#### `wchar_t *_MCFCRT_itow_X(wchar_t *buffer, uintptr_t value);`

* **Description**

    These functions are efficient versions of their `_MCFCRT_itow0*()` counterparts with the `min_digits` parameter set to `0`.

* **Parameters**

    `wchar_t *buffer`  
    The location where the converted string will be written to.  

    `intptr_t value` or `uintptr_t value`  
    The integer to convert.  

* **Return Value**

    These functions return a pointer to the next character of the last one of the converted string.

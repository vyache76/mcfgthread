# env/pp.h

## Macros

#### `_MCFCRT_PP_STRINGIFY(...)`

* **Description**

    This macro expands its parameters to a narrow string literal.

#### `_MCFCRT_PP_WIDEN(x)`
#### `_MCFCRT_PP_UTF8(x)`
#### `_MCFCRT_PP_UTF16(x)`
#### `_MCFCRT_PP_UTF32(x)`

* **Description**

    These macros prepends their parameters, which must be narrow string literals, with `L`, `u8`, `u` and `U` respectively, resulting in wide, UTF-8, UTF-16 and UTF-32 string literals.

#### `_MCFCRT_PP_FIRST(f, ...)`

* **Description**

    This macro takes at least one parameter and expands to the first one of them.

#### `_MCFCRT_PP_REST(f, ...)`

* **Description**

    This macro takes at least one parameter and expands to all but the first one of them.

#### `_MCFCRT_PP_CAT2(x, y)`
#### `_MCFCRT_PP_CAT3(x, y, z)`

* **Description**

    This macro takes some pp-tokens and concatenate them to make a new pp-token.

#### `_MCFCRT_PP_LAZY(f, ...)`

* **Description**

    This macro expands to `f(...)` as if `f` were a pre-processor level function.

## Types

None.

## Functions

None.

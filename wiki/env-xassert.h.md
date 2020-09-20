# env/xassert.h

## Macros

#### `void _MCFCRT_ASSERT_MSG(`_EXPRESSION_` expr, const wchar_t *msg)`

* **Description**

    This macro evaluates its `expr` parameter and contextually converts the result to type `bool`. After the conversion:
    * If the result is `true`, this macro does nothing.  
    * Otherwise, if the macro `NDEBUG` is not defined prior to the last inclusion of this header, `_MCFCRT_Bail()` is called to terminate the process with a message formatted from `expr`, `__FILE__`, `__LINE__` and `msg`.  
    * Otherwise, the behavior is undefined.

    Particularly, the following code gives the compiler the right to assume the controlling expression of the `if` statement is never `false` (i.e. the pointer `p` is never `NULL`):

        ```c
    // gcc -O3 test.c
    #define NDEBUG 1
    #include <mcfgthread/ext/assert.h>
    #include <stdio.h>

    int *p = 0;

    int main(){
        _MCFCRT_ASSERT(p);
        if(p){
            puts("boom!");
        }
    }
    ```

#### `void _MCFCRT_ASSERT(`_EXPRESSION_` expr)`

* **Description**

    This macro is equivalent to `_MCFCRT_ASSERT_MSG()` with the `msg` parameter set to `L""`.

## Types

None.

## Functions

None.

# env/expect.h

## Macros

#### `bool _MCFCRT_EXPECT(`_EXPRESSION_` expr_);`

* **Description**

    This macro evaluates its `expr_` parameter, converts it to `bool`, then returns the result.  
    When used inside a controlling expression it give the optimizer the hint that the expression is less likely to be `false`.

    An example would be:

    ```c
    char *strdup(const char *s){
        const size_t len = strlen(s);
        char *const p = malloc(len + 1);
        if(_MCFCRT_EXPECT(p)){
            memcpy(p, s, len + 1);
        }
        return p;
    }
    ```

#### `bool _MCFCRT_EXPECT_NOT(`_EXPRESSION_` expr_);`

* **Description**

    This macro is similar to `_MCFCRT_EXPECT()` except that it give the optimizer the hint that the expression is more likely to be `false`.

## Types

None.

## Functions

None.

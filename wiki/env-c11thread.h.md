# env/c11thread.h

## Macros

#### `#define thread_local _Thread_local`
#### `#define ONCE_FLAG_INIT { 0 }`
#### `#define TSS_DTOR_ITERATIONS 1`

* **Description**

    See [Thread support library](http://en.cppreference.com/w/c/thread).

    The `thread_local` macro is **only** defined when compiled as C.

## Types

#### `typedef uintptr_t thrd_t;`
#### `typedef int (*thrd_start_t)(void *);`
#### `typedef struct __MCFCRT_tagC11threadConditionVariable cnd_t;`
#### `typedef struct __MCFCRT_tagC11threadMutex mtx_t;`
#### `typedef _MCFCRT_TlsKeyHandle tss_t; `
#### `typedef void (*tss_dtor_t)(void *);`
#### `typedef union __MCFCRT_tagC11threadOnceFlag once_flag;`

* **Description**

    See [Thread support library](http://en.cppreference.com/w/c/thread).

## Enumeration Constants

```c
enum {
    mtx_plain     = 0x0000,
    mtx_recursive = 0x0001,
    mtx_timed     = 0x0002,

    thrd_timedout = ETIMEDOUT,
    thrd_success  = 0,
    thrd_busy     = EBUSY,
    thrd_error    = EPERM,
    thrd_nomem    = ENOMEM,
};
```

* **Description**

    See [Thread support library](http://en.cppreference.com/w/c/thread).

    As C11 requires certain functions to return `thrd_error` on error but POSIX requires them to return error codes other than `EPERM`, users are not suggested to use an error code in the same way as if it were obtained from `errno`.

## Functions

#### `void call_once(once_flag *once, void (*func)(void));`
#### `int cnd_init(cnd_t *cond);`
#### `void cnd_destroy(cnd_t *cond);`
#### `int cnd_timedwait(cnd_t *restrict cond, mtx_t *restrict mutex, const struct timespec *restrict timeout);`
#### `int cnd_wait(cnd_t *restrict cond, mtx_t *restrict mutex);`
#### `int cnd_signal(cnd_t *cond);`
#### `int cnd_broadcast(cnd_t *cond);`
#### `int mtx_init(mtx_t *mutex, int mask);`
#### `void mtx_destroy(mtx_t *mutex);`
#### `int mtx_lock(mtx_t *mutex);`
#### `int mtx_timedlock(mtx_t *restrict mutex, const struct timespec *restrict timeout);`
#### `int mtx_trylock(mtx_t *mutex);`
#### `int mtx_unlock(mtx_t *mutex);`
#### `int thrd_create(thrd_t *tid_ret, thrd_start_t proc, void *param);`
#### `noreturn void thrd_exit(int exit_code);`
#### `int thrd_join(thrd_t tid, int *exit_code_ret);`
#### `int thrd_detach(thrd_t tid);`
#### `thrd_t thrd_current(void);`
#### `int thrd_equal(thrd_t tid1, thrd_t tid2);`
#### `int thrd_sleep(const struct timespec *duration, struct timespec *remaining);`
#### `void thrd_yield(void);`
#### `int tss_create(tss_t *key_ret, tss_dtor_t destructor);`
#### `void tss_delete(tss_t key);`
#### `void *tss_get(tss_t key);`
#### `int tss_set(tss_t key, void *value);`

* **Description**

    See [Thread support library](http://en.cppreference.com/w/c/thread).

* **Parameters**

    See [Thread support library](http://en.cppreference.com/w/c/thread).

* **Return Value**

    See [Thread support library](http://en.cppreference.com/w/c/thread).

    The `thrd_exit()` function cannot return.

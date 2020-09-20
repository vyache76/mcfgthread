# env/gthread.h

# Macros

#### `#define __GTHREADS 1`

* **Description**

    This macro tells GCC libraries that gthread is supported.

#### `#define __GTHREAD_HAS_COND 1`

* **Description**

    This macro tells GCC libraries that gthread condition variable is supported.

#### `#define __GTHREAD_ONCE_INIT { 0 }`
#### `#define __GTHREAD_MUTEX_INIT { 0 }`
#### `#define __GTHREAD_RECURSIVE_MUTEX_INIT { 0, 0, __GTHREAD_MUTEX_INIT }`
#### `#define __GTHREAD_COND_INIT { 0 }`

* **Description**

    These macros are for static initialization of once-initialization flags, mutexes, recursive mutexes and condition variables, respectively.

#### `#define __GTHREAD_MUTEX_INIT_FUNCTION __gthread_mutex_init_function`
#### `#define __GTHREAD_RECURSIVE_MUTEX_INIT_FUNCTION __gthread_recursive_mutex_init_function`
#### `#define __GTHREAD_COND_INIT_FUNCTION __gthread_cond_init_function`

* **Description**

    These macros are for dynamic initialization of mutexes, recursive mutexes and condition variables, respectively.

## Types

#### `typedef _MCFCRT_TlsKeyHandle __gthread_key_t;`

* **Description**

    This is the type alias for GCC libraries to use thread local storage.

#### `typedef _MCFCRT_OnceFlag __gthread_once_t;`

* **Description**

    This is the type alias for GCC libraries to use once-initialization.

#### `typedef _MCFCRT_Mutex __gthread_mutex_t;`

* **Description**

    This is the type alias for GCC libraries to use mutexes.

#### `typedef struct __MCFCRT_tagGthreadRecursiveMutex __gthread_recursive_mutex_t;`

* **Description**

    This is the type alias for GCC libraries to use recursive mutexes.

#### `typedef _MCFCRT_ConditionVariable __gthread_cond_t;`

* **Description**

    This is the type alias for GCC libraries to use condition variables.

#### `typedef uintptr_t __gthread_t;`

* **Description**

    This is the type alias for GCC libraries to store thread IDs.

## Functions

#### `int __gthread_active_p(void);`

* **Description**

    This function tells GCC libraries whether the current execution environment is multi-threaded.

* **Parameters**

    This function does not take any parameters.

* **Return Value**

    This function always returns `1`.

* **Remarks**

    Since C++11 this function is marked as `constexpr`.

#### `int __gthread_key_create(__gthread_key_t *__key_ret, void (*__destructor)(void *));`

* **Description**

    This function creates a TLS key for thread local object access.

* **Parameters**

    `__gthread_key_t *__key_ret`  
    The location where to store the newly allocated TLS key.

    `void (*__destructor)(void *)`  
    A callback function for TLS cleanup. This parameter can be `NULL`.
    If this parameter is `NULL`, no cleanup is performed.

* **Return Value**

    If the function succeeds, the return value is zero.
    If the TLS key cannot be allocated, the return value is `ENOMEM`.

#### `int __gthread_key_delete(__gthread_key_t __key);`

* **Description**

    This function deletes the specified TLS key allocated by `__gthread_key_create()`.

* **Parameters**

    `__gthread_key_t __key`  
    The TLS key to delete.

* **Return Value**

    This function always returns zero.

#### `void *__gthread_getspecific(__gthread_key_t __key);`

* **Description**

    This function gets the thread local pointer referred by the specified key.

* **Parameters**

    `__gthread_key_t __key`  
    The TLS key allocated by `__gthread_key_create()`.
    If the TLS key has already been freed using `__gthread_key_delete()`, the behavior is undefined.

* **Return Value**

    This function returns the last value that `__gthread_setspecific()` is called to set with the same key.
    If `__gthread_setspecific()` has not been called to set a value with this key, `NULL` is returned.

* **Remarks**

    This function provides no error checking.

#### `int __gthread_setspecific(__gthread_key_t __key, const void *__value);`

* **Description**

    This function sets the thread local pointer referred by the specified key.

* **Parameters**

    `__gthread_key_t __key`  
    The TLS key allocated by `__gthread_key_create()`.
    If the TLS key has already been freed using `__gthread_key_delete()`, the behavior is undefined.

    `const void *__value`  
    The value to set. After a successful call to this function, `__gthread_getspecific(__key)` returns this value.

* **Return Value**

    If the function succeeds, the return value is zero.
    If the storage for the thread local pointer cannot be allocated, the return value is `ENOMEM`.

#### `int __gthread_once(__gthread_once_t *__flag, void (*__func)(void));`

* **Description**

    When multiple threads calls this function with the same `__flag` parameter simultaneously, the first thread calls `(*__func)()` and the other threads are suspended. After the first thread returns from the callback, the other threads are woken up and resume execution and no thread will be suspended on this once-initialization flag any more.
    If the first thread calls `__gthread_once()` with the same `__flag` parameter before it returns from the callback, the behavior is undefined.

* **Parameters**

    `__gthread_once_t *__flag`  
    The once-initialization flag providing synchronization.

    `void (*__func)(void)`  
    The callback to invoke exactly once.

* **Return Value**

    This function always returns `0`.

* **Remarks**

    There is no way for `__func` to report failure, including throwing exceptions.

#### `void __gthread_mutex_init_function(__gthread_mutex_t *__mutex);`

* **Description**

    This function initializes a mutex.

* **Parameters**

    `__gthread_mutex_t *__mutex`  
    The mutex to initialize.

* **Return Value**

    This function does not return a value.

#### `int __gthread_mutex_destroy(__gthread_mutex_t *__mutex);`

* **Description**

    This function does nothing.

* **Paramaters**

    `__gthread_mutex_t *__mutex`  
    This parameter is ignored.

* **Return Value**

    This function always returns `0`.

#### `int __gthread_mutex_trylock(__gthread_mutex_t *__mutex);`

* **Description**

    This function checks whether the specified mutex is unlocked and, if it is unlocked, locks it.
    If the mutex has been locked by another thread, this function returns immediately with an error.
    If the mutex has been locked by the current thread, the behavior is undefined.

* **Parameters**

    `__gthread_mutex_t *__mutex`  
    The mutex to try locking.

* **Return Value**

    If the mutex is successfully locked by the current thread, this function returns `0`.
    Otherwise, this function returns `EBUSY`.

#### `int __gthread_mutex_lock(__gthread_mutex_t *__mutex);`

* **Description**

    This function behaves the same as `__gthread_mutex_trylock()` if the mutex would be locked by the current thread.
    If the mutex has been locked by another thread, the current thread is suspended until another thread unlocks the mutex. The contention is then repeated until the current thread locks the mutex.

* **Parameters**

    `__gthread_mutex_t *__mutex`  
    The mutex to lock.

* **Return Value**

    This function always returns `0`.

#### `int __gthread_mutex_unlock(__gthread_mutex_t *__mutex);`

* **Description**

    This function unlocks the specified mutex, allowing any thread to lock the mutex again.
    If the specified mutex has not been locked by any thread, the behavior is undefined.

* **Parameters**

    `__gthread_mutex_t *__mutex`  
    The mutex to unlock.

* **Return Value**

    This function always returns `0`.

#### `void __gthread_recursive_mutex_init_function(__gthread_recursive_mutex_t *__recur_mutex);`

* **Description**

    This function initializes a recursive mutex.

* **Parameters**

    `__gthread_recursive_mutex_t *__recur_mutex`  
    The recursive mutex to initialize.

* **Return Value**

    This function does not return a value.

#### `int __gthread_recursive_mutex_destroy(__gthread_recursive_mutex_t *__recur_mutex);`

* **Description**

    This function does nothing.

* **Parameters**

    `__gthread_recursive_mutex_t *__recur_mutex`  
    This parameter is ignored.

* **Return Value**

    This function always returns `0`.

#### `int __gthread_recursive_mutex_trylock(__gthread_recursive_mutex_t *__recur_mutex);`

* **Description**

    This function checks whether the specified recursive mutex is unlocked and, if it is unlocked, locks it.
    If the recursive mutex has been locked by another thread, this function returns immediately with an error.
    If the recursive mutex has been locked by the current thread, the function increments the _lock count_ and returns `0`. If the _lock count_ overflows, be it signed or unsigned, the behavior is undefined.

* **Parameters**

    `__gthread_recursive_mutex_t *__recur_mutex`  
    The recursive mutex to try locking.

* **Return Value**

    If the mutex is successfully locked by the current thread, this function returns `0`.
    Otherwise, this function returns `EBUSY`.

#### `int __gthread_recursive_mutex_lock(__gthread_recursive_mutex_t *__recur_mutex);`

* **Description**

    This function behaves the same as `__gthread_recursive_mutex_trylock()` if the recursive mutex would be locked by the current thread.
    If the mutex has been locked by another thread, the calling thread is suspended until another thread unlocks the recursive mutex. The contention is then repeated until the current thread locks the recursive mutex.

* **Parameters**

    `__gthread_recursive_mutex_t *__recur_mutex`  
    The recursive mutex to lock.

* **Return Value**

    This function always returns `0`.

#### `int __gthread_recursive_mutex_unlock(__gthread_recursive_mutex_t *__recur_mutex);`

* **Description**

    This function decrements the _lock count_ of the specified recursive mutex. If the _lock count_ drops to zero, the recursive mutex is unlocked, allowing any thread to lock the recursive mutex again.
    If the specified mutex has not been locked by the current thread, the behavior is undefined.

* **Parameters**

    `__gthread_recursive_mutex_t *__recur_mutex`  
    The recursive mutex to unlock.

* **Return Value**

    This function always returns `0`.

#### `void __gthread_cond_init_function(__gthread_cond_t *__cond);`

* **Description**

    This function initializes a condition variable.

* **Parameters**

    `__gthread_cond_t *__cond`  
    The condition variable to initialize.

* **Return Value**

    This function does not return a value.

#### `int __gthread_cond_destroy(__gthread_cond_t *__cond);`

* **Description**

    This function does nothing.

* **Parameters**

    `__gthread_cond_t *__cond`  
    This parameter is ignored.

* **Return Value**

    This function always returns `0`.

#### `int __gthread_cond_wait(__gthread_cond_t *__cond, __gthread_mutex_t *__mutex);`

* **Description**

    This functions atomically unlocks the specified mutex and suspends the current thread, until another thread calls `__gthread_cond_signal()` or `__gthread_cond_broadcast()` with the same `__cond` parameter. The mutex is relocked by the current thread before this function returns.
    If the mutex has not been locked by any thread, the behavior is undefined.

* **Parameters**

    `__gthread_cond_t *__cond`  
    The condition variable to wait for.

    `__gthread_mutex_t *__mutex`  
    The mutex to unlock.

* **Return Value**

    This function always returns `0`.

#### `int __gthread_cond_wait_recursive(__gthread_cond_t *__cond, __gthread_recursive_mutex_t *__recur_mutex);`

* **Description**

    This functions atomically unlocks the specified recursive mutex and suspends the current thread no matter what value the _lock count_ is, until another thread calls `__gthread_cond_signal()` or `__gthread_cond_broadcast()` with the same `__cond` parameter. The recursive mutex is then relocked by the current thread before this function returns with the _lock count_ bumped up to the same value before this function call.
    If the recursive mutex has not been locked by the current thread, the behavior is undefined.

* **Parameters**

    `__gthread_cond_t *__cond`  
    The condition variable to wait for.

    `__gthread_recursive_mutex_t *__recur_mutex`  
    The recursive mutex to unlock.

* **Return Value**

    This function always returns `0`.

#### `int __gthread_cond_signal(__gthread_cond_t *__cond);`

* **Description**

    This function wakes up a thread that has been suspended on the specified condition variable.
    It is suggested that the caller have the corresponding mutex locked while calling this function.

* **Parameters**

    `__gthread_cond_t *__cond`  
    The condition variable which other threads might be suspended on.

* **Return Value**

    This function always returns `0`.

#### `int __gthread_cond_broadcast(__gthread_cond_t *__cond);`

* **Description**

    This function wakes up all threads that have been suspended on the specified condition variable.
    It is suggested that the caller have the corresponding mutex locked while calling this function.

* **Parameters**

    `__gthread_cond_t *__cond`  
    The condition variable which other threads might be suspended on.

* **Return Value**

    This function always returns `0`.

#### `int __gthread_create(__gthread_t *__tid_ret, void *(*__proc)(void *), void *__param);`

* **Description**

    This function creates a `joinable` thread. A `joinable` thread must be either joined or detached exactly once.

* **Parameters**

    `__gthread_t *__tid_ret`  
    The location where to store the thread ID of the newly created thread.
    The thread ID uniquely designates a thread in the operating system before the thread is joined or detached.

* **Return Value**

    This function returns `0` on success.
    If the thread could not be created, this function returns `EAGAIN`.

#### `int __gthread_join(__gthread_t __tid, void **restrict __exit_code_ret);`

* **Description**

    This function joins the current thread with another thread.
    The current thread is suspended until the other thread terminates, if it is still running.
    If the other thread has terminated, this function stores its exit code into `*__exit_code_ret`, cleans up all resources associated with it and returns.
    Only one thread can join with a thread at a time. If there is another thread joining with the specified thread, this function fails.

* **Parameters**

    `__gthread_t __tid`  
    The thread to join.

    `void **restrict __exit_code_ret`  
    The location where to store the exit code of the specified thread. This parameter can be `NULL`.
    If `NULL` is specified, the exit code is not returned.

* **Return Value**

    This function returns `0` on success.
    If the specified thread ID designates the current thread, this function returns `EDEADLK`.
    If the thread is being joined by another thread or has already been detached, this function returns `ESRCH`. This error code is for debug purposes only and should not be relied on since thread IDs can be reused by the operating system.

#### `int __gthread_detach(__gthread_t __tid);`

* **Description**

    This function makes a thread `detached` i.e. no longer `joinable`.
    This function returns immediately without waiting for the other thread to terminate. The resources associated with the other thread are automatically reclaimed when it terminates.

* **Parameters**

    `__gthread_t __tid`  
    The thread to detach.

* **Return Value**

    This function returns `0` on success.
    If the thread is being joined by another thread or has already been detached, this function returns `ESRCH`. This error code is for debug purposes only and should not be relied on since thread IDs can be reused by the operating system.

#### `int __gthread_equal(__gthread_t __tid1, __gthread_t __tid2);`

* **Description**

    This function compares two thread IDs.

* **Parameters**

    `__gthread_t __tid1` and `__gthread_t __tid2`  
    A pair of thread IDs to compare.

* **Return Value**

    This function returns `true` if the two thread IDs compare equal, and `false` otherwise.

* **Remarks**

    This function is identical to `(int)(__tid1 == __tid2)`.

#### `__gthread_t __gthread_self(void);`

* **Description**

    This function returns the thread ID of the current thread.

* **Parameters**

    This function takes no parameters.

* **Return Value**

    This function returns the thread ID of the current thread.

#### `int __gthread_yield(void);`

* **Description**

    This function yields the execution of the current thread. It causes the calling thread to relinquish the CPU for another thread to run.

* **Parameters**

    This function takes no parameters.

* **Return Value**

    This function always returns `0`.

#### `int __gthread_mutex_timedlock(__gthread_mutex_t *__mutex, const __gthread_time_t *__timeout);`

* **Description**

    This function checks whether the specified mutex is unlocked and, if it is unlocked, locks it.
    If the mutex has been locked by another thread, this function waits for it to unlock the mutex until the specified time point.
    If the mutex has been locked by the current thread, the behavior is undefined.

* **Parameters**

    `__gthread_mutex_t *__mutex`  
    The mutex to try locking.

    `const __gthread_time_t *__timeout`  
    The time point when the wait would time out, should the mutex be still held by any thread.

* **Return Value**

    If the mutex is successfully locked by the current thread, this function returns `0`.
    If the wait has timed out, this function returns `ETIMEDOUT`.

#### `int __gthread_recursive_mutex_timedlock(__gthread_recursive_mutex_t *__recur_mutex, const __gthread_time_t *__timeout);`

* **Description**

    This function checks whether the specified recursive mutex is unlocked and, if it is unlocked, locks it.
    If the recursive mutex has been locked by another thread, this function waits for it to unlock the recursive mutex until the specified time point.
    If the recursive mutex has been locked by the current thread, the function increments the _lock count_ and returns `0`. If the _lock count_ overflows, be it signed or unsigned, the behavior is undefined.

* **Parameters**

    `__gthread_recursive_mutex_t *__recur_mutex`  
    The recursive mutex to try locking.

    `const __gthread_time_t *__timeout`  
    The time point when the wait would time out, should the recursive mutex be still held by another thread.

* **Return Value**

    If the recursive mutex is successfully locked by the current thread, this function returns `0`.
    If the wait has timed out, this function returns `ETIMEDOUT`.

#### `int __gthread_cond_timedwait(__gthread_cond_t *__cond, __gthread_mutex_t *__mutex, const __gthread_time_t *__timeout);`

* **Description**

    This functions atomically unlocks the specified mutex and suspends the current thread, until another thread calls `__gthread_cond_signal()` or `__gthread_cond_broadcast()` with the same `__cond` parameter  until the specified time point. The mutex is relocked by the current thread before this function returns.
    If the mutex has not been locked by any thread, the behavior is undefined.

* **Parameters**

    `__gthread_cond_t *__cond`  
    The condition variable to wait for.

    `__gthread_mutex_t *__mutex`  
    The mutex to unlock.

    `const __gthread_time_t *__timeout`  
    The time point when the wait would time out, should the condition variable not be signaled by any other thread.

* **Return Value**

    This function returns `0` if the wait has not timed out. That is, the thread has been woken up by any or no thread (spuriously).
    If the wait has timed out, the return value is `ETIMEDOUT`.

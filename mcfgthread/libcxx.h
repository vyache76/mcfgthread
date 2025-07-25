/* This file is part of MCF Gthread.
 * Copyright (C) 2022-2025 LH_Mouse. All wrongs reserved.
 *
 * MCF Gthread is free software. Licensing information is included in
 * LICENSE.TXT as a whole. The GCC Runtime Library Exception applies
 * to this file.  */

#ifndef __MCFGTHREAD_LIBCXX_
#define __MCFGTHREAD_LIBCXX_

#include "fwd.h"
#include "gthr_aux.h"
#include <errno.h>

__MCF_CXX(namespace std {)
__MCF_CXX(extern "C" {)
#ifndef __MCF_LIBCXX_IMPORT
#  define __MCF_LIBCXX_IMPORT
#  define __MCF_LIBCXX_INLINE  __MCF_GNU_INLINE
#endif

/* Define threading support types for libc++.  */
typedef __MCF_timespec __libcpp_timespec_t;
typedef _MCF_thread* __libcpp_thread_t;
typedef uint32_t __libcpp_thread_id;
typedef _MCF_tls_key* __libcpp_tls_key;

typedef _MCF_once __libcpp_exec_once_flag;
typedef _MCF_cond __libcpp_condvar_t;
typedef _MCF_mutex __libcpp_mutex_t;
typedef __MCF_gthr_rc_mutex __libcpp_recursive_mutex_t;

/* Define macros for initialization.  */
#define _LIBCPP_NULL_THREAD   __MCF_nullptr
#define _LIBCPP_TLS_DESTRUCTOR_CC  /* default calling convention  */
#define _LIBCPP_EXEC_ONCE_INITIALIZER   __MCF_0_INIT
#define _LIBCPP_CONDVAR_INITIALIZER   __MCF_0_INIT
#define _LIBCPP_MUTEX_INITIALIZER   __MCF_0_INIT

/* Performs one-time initialization, like `pthread_once()`.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_execute_once(__libcpp_exec_once_flag* __once, __MCF_once_callback* __init_proc)
  __MCF_MAY_THROW;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_execute_once, __libcpp_execute_once);
#  define __MCF_libcxx_execute_once  __libcpp_execute_once
#endif

/* Allocates a thread-specific key, like `pthread_key_create()`.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_tls_create(__libcpp_tls_key* __keyp, _MCF_tls_dtor* __dtor_opt)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_tls_create, __libcpp_tls_create);
#  define __MCF_libcxx_tls_create  __libcpp_tls_create
#endif

/* Destroys a thread-specific key, like `pthread_key_delete()`.
 * This function is currently unused.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_tls_delete(__libcpp_tls_key __key)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_tls_delete, __libcpp_tls_delete);
#  define __MCF_libcxx_tls_delete  __libcpp_tls_delete
#endif

/* Gets a thread-specific value, like `pthread_getspecific()`.  */
__MCF_LIBCXX_IMPORT __MCF_FN_PURE
void*
__MCF_libcxx_tls_get(__libcpp_tls_key __key)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_tls_get, __libcpp_tls_get);
#  define __MCF_libcxx_tls_get  __libcpp_tls_get
#endif

/* Sets a thread-specific value, like `pthread_setspecific()`.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_tls_set(__libcpp_tls_key __key, const void* __val_opt)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_tls_set, __libcpp_tls_set);
#  define __MCF_libcxx_tls_set  __libcpp_tls_set
#endif

/* Initializes a mutex, like `pthread_mutex_init()`.
 * This function is currently unused.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_mutex_init(__libcpp_mutex_t* __mtx)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_mutex_init, __libcpp_mutex_init);
#  define __MCF_libcxx_mutex_init  __libcpp_mutex_init
#endif

/* Destroys a mutex. This function does nothing.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_mutex_destroy(__libcpp_mutex_t* __mtx)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_mutex_destroy, __libcpp_mutex_destroy);
#  define __MCF_libcxx_mutex_destroy  __libcpp_mutex_destroy
#endif

/* Locks a mutex, like `pthread_mutex_lock()`.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_mutex_lock(__libcpp_mutex_t* __mtx)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_mutex_lock, __libcpp_mutex_lock);
#  define __MCF_libcxx_mutex_lock  __libcpp_mutex_lock
#endif

/* Tries locking a mutex without blocking, like `pthread_mutex_trylock()`.  */
__MCF_LIBCXX_IMPORT
bool
__MCF_libcxx_mutex_trylock(__libcpp_mutex_t* __mtx)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_mutex_trylock, __libcpp_mutex_trylock);
#  define __MCF_libcxx_mutex_trylock  __libcpp_mutex_trylock
#endif

/* Unlocks a mutex, like `pthread_mutex_unlock()`.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_mutex_unlock(__libcpp_mutex_t* __mtx)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_mutex_unlock, __libcpp_mutex_unlock);
#  define __MCF_libcxx_mutex_unlock  __libcpp_mutex_unlock
#endif

/* Initializes a recursive mutex, like `pthread_mutex_init()`.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_recursive_mutex_init(__libcpp_recursive_mutex_t* __mtx)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_recursive_mutex_init, __libcpp_recursive_mutex_init);
#  define __MCF_libcxx_recursive_mutex_init  __libcpp_recursive_mutex_init
#endif

/* Destroys a recursive mutex. This function does nothing.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_recursive_mutex_destroy(__libcpp_recursive_mutex_t* __rmtx)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_recursive_mutex_destroy, __libcpp_recursive_mutex_destroy);
#  define __MCF_libcxx_recursive_mutex_destroy  __libcpp_recursive_mutex_destroy
#endif

/* Locks a recursive mutex, like `pthread_mutex_lock()`.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_recursive_mutex_lock(__libcpp_recursive_mutex_t* __rmtx)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_recursive_mutex_lock, __libcpp_recursive_mutex_lock);
#  define __MCF_libcxx_recursive_mutex_lock  __libcpp_recursive_mutex_lock
#endif

/* Tries locking a recursive mutex without blocking, like
 * `pthread_mutex_trylock()`.  */
__MCF_LIBCXX_IMPORT
bool
__MCF_libcxx_recursive_mutex_trylock(__libcpp_recursive_mutex_t* __rmtx)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_recursive_mutex_trylock, __libcpp_recursive_mutex_trylock);
#  define __MCF_libcxx_recursive_mutex_trylock  __libcpp_recursive_mutex_trylock
#endif

/* Unlocks a recursive mutex, like `pthread_mutex_unlock()`.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_recursive_mutex_unlock(__libcpp_recursive_mutex_t* __rmtx)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_recursive_mutex_unlock, __libcpp_recursive_mutex_unlock);
#  define __MCF_libcxx_recursive_mutex_unlock  __libcpp_recursive_mutex_unlock
#endif

/* Initializes a condition variable, like `pthread_cond_init()`.
 * This function is currently unused.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_condvar_init(__libcpp_condvar_t* __cond)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_condvar_init, __libcpp_condvar_init);
#  define __MCF_libcxx_condvar_init  __libcpp_condvar_init
#endif

/* Destroys a condition variable. This function does nothing.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_condvar_destroy(__libcpp_condvar_t* __cond)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_condvar_destroy, __libcpp_condvar_destroy);
#  define __MCF_libcxx_condvar_destroy  __libcpp_condvar_destroy
#endif

/* Waits for a condition variable, like `pthread_cond_wait()`.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_condvar_wait(__libcpp_condvar_t* __cond, __libcpp_mutex_t* __mtx)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_condvar_wait, __libcpp_condvar_wait);
#  define __MCF_libcxx_condvar_wait  __libcpp_condvar_wait
#endif

/* Waits for a condition variable until a time point, like
 * `pthread_cond_timedwait()`.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_condvar_timedwait(__libcpp_condvar_t* __cond, __libcpp_mutex_t* __mtx,
                               const __libcpp_timespec_t* __abs_time)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_condvar_timedwait, __libcpp_condvar_timedwait);
#  define __MCF_libcxx_condvar_timedwait  __libcpp_condvar_timedwait
#endif

/* Signals at most one thread that is waiting on the condition variable, like
 * `pthread_cond_signal()`.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_condvar_signal(__libcpp_condvar_t* __cond)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_condvar_signal, __libcpp_condvar_signal);
#  define __MCF_libcxx_condvar_signal  __libcpp_condvar_signal
#endif

/* Signals all threads that are waiting on the condition variable, like
 * `pthread_cond_broadcast()`.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_condvar_broadcast(__libcpp_condvar_t* __cond)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_condvar_broadcast, __libcpp_condvar_broadcast);
#  define __MCF_libcxx_condvar_broadcast  __libcpp_condvar_broadcast
#endif

/* Creates a thread, like `pthread_create()`.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_thread_create(__libcpp_thread_t* __thrdp, __MCF_gthr_thread_procedure* __proc, void* __arg)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_thread_create, __libcpp_thread_create);
#  define __MCF_libcxx_thread_create  __libcpp_thread_create
#endif

/* Awaits a thread to terminate and gets its result, like `pthread_join()`.  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_thread_join(const __libcpp_thread_t* __thrdp)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_thread_join, __libcpp_thread_join);
#  define __MCF_libcxx_thread_join  __libcpp_thread_join
#endif

/* Detaches a thread, like `pthread_detach()`  */
__MCF_LIBCXX_IMPORT
int
__MCF_libcxx_thread_detach(const __libcpp_thread_t* __thrdp)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_thread_detach, __libcpp_thread_detach);
#  define __MCF_libcxx_thread_detach  __libcpp_thread_detach
#endif

/* Checks whether a thread object is null.  */
__MCF_LIBCXX_IMPORT __MCF_FN_PURE
bool
__MCF_libcxx_thread_isnull(const __libcpp_thread_t* __thrdp)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_thread_isnull, __libcpp_thread_isnull);
#  define __MCF_libcxx_thread_isnull  __libcpp_thread_isnull
#endif

/* Gets a thread itself, like `pthread_self()`.
 * The thread shall be the main thread, or shall have been created by
 * `__libcpp_thread_create()`. Otherwise the behavior is undefined.  */
__MCF_LIBCXX_IMPORT __MCF_FN_CONST
__libcpp_thread_id
__MCF_libcxx_thread_get_current_id(void)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_thread_get_current_id, __libcpp_thread_get_current_id);
#  define __MCF_libcxx_thread_get_current_id  __libcpp_thread_get_current_id
#endif

/* Gets the ID of another thread.  */
__MCF_LIBCXX_IMPORT __MCF_FN_PURE
__libcpp_thread_id
__MCF_libcxx_thread_get_id(const __libcpp_thread_t* __thrdp)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_thread_get_id, __libcpp_thread_get_id);
#  define __MCF_libcxx_thread_get_id  __libcpp_thread_get_id
#endif

/* Checks whether two thread IDs compare equal, like `pthread_equal()`.  */
__MCF_LIBCXX_IMPORT __MCF_FN_CONST
bool
__MCF_libcxx_thread_id_equal(__libcpp_thread_id __t1, __libcpp_thread_id __t2)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_thread_id_equal, __libcpp_thread_id_equal);
#  define __MCF_libcxx_thread_id_equal  __libcpp_thread_id_equal
#endif

/* Checks whether two thread IDs compare less, for standard containers.  */
__MCF_LIBCXX_IMPORT __MCF_FN_CONST
bool
__MCF_libcxx_thread_id_less(__libcpp_thread_id __t1, __libcpp_thread_id __t2)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_thread_id_less, __libcpp_thread_id_less);
#  define __MCF_libcxx_thread_id_less  __libcpp_thread_id_less
#endif

/* Suspends execution of the current thread for an amount of time.  */
__MCF_LIBCXX_IMPORT
void
__MCF_libcxx_thread_sleep_for_ns_count(int64_t __ns)
  __MCF_noexcept;

#ifdef _LIBCPP___CHRONO_DURATION_H
__MCF_ALWAYS_INLINE
void
__libcpp_thread_sleep_for(const chrono::nanoseconds& __ns)
  __MCF_noexcept
  {
    __MCF_libcxx_thread_sleep_for_ns_count(__ns.count());
  }
#endif  /* <chrono> included  */

/* Gives up the current time slice, like `sched_yield()`.  */
__MCF_LIBCXX_IMPORT
void
__MCF_libcxx_thread_yield(void)
  __MCF_noexcept;

#ifndef __MCF_LIBCXX_NO_ALIASES
__MCF_FNA(__MCF_libcxx_thread_yield, __libcpp_thread_yield);
#  define __MCF_libcxx_thread_yield  __libcpp_thread_yield
#endif

/* Define inline functions after all declarations.
 * We would like to keep them away from declarations for conciseness, which also
 * matches the disposition of non-inline functions. Note that however, unlike C++
 * inline functions, they have to have consistent inline specifiers throughout
 * this file.  */
__MCF_LIBCXX_INLINE
int
__MCF_libcxx_execute_once(__libcpp_exec_once_flag* __once, __MCF_once_callback* __init_proc)
  __MCF_MAY_THROW
  {
    __MCF_GTHR_CALL_ONCE_SEH(__once, __init_proc, __MCF_nullptr);
    return 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_tls_create(__libcpp_tls_key* __keyp, _MCF_tls_dtor* __dtor_opt)
  __MCF_noexcept
  {
    _MCF_tls_key* __key = _MCF_tls_key_new(__dtor_opt);
    *__keyp = __key;
    return (__key == __MCF_nullptr) ? ENOMEM : 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_tls_delete(__libcpp_tls_key __key)
  __MCF_noexcept
  {
    _MCF_tls_key_delete(__key);
    return 0;
  }

__MCF_LIBCXX_INLINE __MCF_FN_PURE
void*
__MCF_libcxx_tls_get(__libcpp_tls_key __key)
  __MCF_noexcept
  {
    return _MCF_tls_get(__key);
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_tls_set(__libcpp_tls_key __key, const void* __val_opt)
  __MCF_noexcept
  {
    int __err = _MCF_tls_set(__key, __val_opt);
    return (__err != 0) ? EINVAL : 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_mutex_init(__libcpp_mutex_t* __mtx)
  __MCF_noexcept
  {
    _MCF_mutex_init(__mtx);
    return 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_mutex_destroy(__libcpp_mutex_t* __mtx)
  __MCF_noexcept
  {
    (void) __mtx;
    return 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_mutex_lock(__libcpp_mutex_t* __mtx)
  __MCF_noexcept
  {
    int __err = _MCF_mutex_lock(__mtx, __MCF_nullptr);
    __MCF_ASSERT(__err == 0);
    return 0;
  }

__MCF_LIBCXX_INLINE
bool
__MCF_libcxx_mutex_trylock(__libcpp_mutex_t* __mtx)
  __MCF_noexcept
  {
    int64_t __timeout = 0;
    int __err = _MCF_mutex_lock(__mtx, &__timeout);
    return __err == 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_mutex_unlock(__libcpp_mutex_t* __mtx)
  __MCF_noexcept
  {
    _MCF_mutex_unlock(__mtx);
    return 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_recursive_mutex_init(__libcpp_recursive_mutex_t* __rmtx)
  __MCF_noexcept
  {
    __MCF_gthr_rc_mutex_init(__rmtx);
    return 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_recursive_mutex_destroy(__libcpp_recursive_mutex_t* __rmtx)
  __MCF_noexcept
  {
    (void) __rmtx;
    return 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_recursive_mutex_lock(__libcpp_recursive_mutex_t* __rmtx)
  __MCF_noexcept
  {
    int __err = __MCF_gthr_rc_mutex_recurse(__rmtx);
    if(__err == 0)
      return 0;

    __err = __MCF_gthr_rc_mutex_wait(__rmtx, __MCF_nullptr);
    __MCF_ASSERT(__err == 0);
    return 0;
  }

__MCF_LIBCXX_INLINE
bool
__MCF_libcxx_recursive_mutex_trylock(__libcpp_recursive_mutex_t* __rmtx)
  __MCF_noexcept
  {
    int64_t __timeout;
    int __err = __MCF_gthr_rc_mutex_recurse(__rmtx);
    if(__err == 0)
      return true;

    __timeout = 0;
    __err = __MCF_gthr_rc_mutex_wait(__rmtx, &__timeout);
    return __err == 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_recursive_mutex_unlock(__libcpp_recursive_mutex_t* __rmtx)
  __MCF_noexcept
  {
    __MCF_gthr_rc_mutex_release(__rmtx);
    return 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_condvar_init(__libcpp_condvar_t* __cond)
  __MCF_noexcept
  {
    _MCF_cond_init(__cond);
    return 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_condvar_destroy(__libcpp_condvar_t* __cond)
  __MCF_noexcept
  {
    (void) __cond;
    return 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_condvar_wait(__libcpp_condvar_t* __cond, __libcpp_mutex_t* __mtx)
  __MCF_noexcept
  {
    int __err = __MCF_gthr_cond_mutex_wait(__cond, __mtx, __MCF_nullptr);
    __MCF_ASSERT(__err == 0);
    return 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_condvar_timedwait(__libcpp_condvar_t* __cond, __libcpp_mutex_t* __mtx,
                               const __libcpp_timespec_t* __abs_time)
  __MCF_noexcept
  {
    int64_t __timeout = __MCF_gthr_timeout_from_timespec(__abs_time);
    int __err = __MCF_gthr_cond_mutex_wait(__cond, __mtx, &__timeout);
    return (__err != 0) ? ETIMEDOUT : 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_condvar_signal(__libcpp_condvar_t* __cond)
  __MCF_noexcept
  {
    _MCF_cond_signal(__cond);
    return 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_condvar_broadcast(__libcpp_condvar_t* __cond)
  __MCF_noexcept
  {
    _MCF_cond_signal_all(__cond);
    return 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_thread_create(__libcpp_thread_t* __thrdp, __MCF_gthr_thread_procedure* __proc, void* __arg)
  __MCF_noexcept
  {
    *__thrdp = __MCF_gthr_thread_create_v3(__proc, __arg);
    return (*__thrdp == __MCF_nullptr) ? EAGAIN : 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_thread_join(const __libcpp_thread_t* __thrdp)
  __MCF_noexcept
  {
    __MCF_gthr_thread_join_v3(*__thrdp, __MCF_nullptr);
    return 0;
  }

__MCF_LIBCXX_INLINE
int
__MCF_libcxx_thread_detach(const __libcpp_thread_t* __thrdp)
  __MCF_noexcept
  {
    _MCF_thread_drop_ref(*__thrdp);
    return 0;
  }

__MCF_LIBCXX_INLINE __MCF_FN_PURE
bool
__MCF_libcxx_thread_isnull(const __libcpp_thread_t* __thrdp)
  __MCF_noexcept
  {
    return *__thrdp == _LIBCPP_NULL_THREAD;
  }

__MCF_LIBCXX_INLINE __MCF_FN_CONST
__libcpp_thread_id
__MCF_libcxx_thread_get_current_id(void)
  __MCF_noexcept
  {
    return _MCF_thread_self_tid();
  }

__MCF_LIBCXX_INLINE __MCF_FN_PURE
__libcpp_thread_id
__MCF_libcxx_thread_get_id(const __libcpp_thread_t* __thrdp)
  __MCF_noexcept
  {
    return _MCF_thread_get_tid(*__thrdp);
  }

__MCF_LIBCXX_INLINE __MCF_FN_CONST
bool
__MCF_libcxx_thread_id_equal(__libcpp_thread_id __t1, __libcpp_thread_id __t2)
  __MCF_noexcept
  {
    return __t1 == __t2;
  }

__MCF_LIBCXX_INLINE __MCF_FN_CONST
bool
__MCF_libcxx_thread_id_less(__libcpp_thread_id __t1, __libcpp_thread_id __t2)
  __MCF_noexcept
  {
    return __t1 < __t2;
  }

__MCF_LIBCXX_INLINE
void
__MCF_libcxx_thread_yield(void)
  __MCF_noexcept
  {
    _MCF_yield();
  }

__MCF_CXX(})  /* extern "C"  */
__MCF_CXX(})  /* namespace std  */
#endif  /* __MCFGTHREAD_LIBCXX_  */

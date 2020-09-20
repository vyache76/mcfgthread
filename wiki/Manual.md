# Manual

All APIs are cataloged here by files where they reside.

* [[env/avl_tree.h]]  
This header provides support for [AVL trees](https://en.wikipedia.org/wiki/AVL_tree).

* [[env/bail.h]]  
This header provides abnormal termination support.

* [[env/c11thread.h]]  
This header provides ISO/IEC C11 wrappers for other headers. 

* [[env/clocks.h]]  
This header provides time support and is required when using timed wait functions.

* [[env/condition_variable.h]]  
This header provides APIs for [condition variables](https://en.wikipedia.org/wiki/Condition_variable).

* [[env/crt_module.h]]  
This header provides APIs for process termination support.

* [[env/expect.h]]  
This header provides macros for static branch prediction support.

* [[env/gthread.h]]  
This header provides `__gthread_*` wrappers for other headers. 

* [[env/heap.h]]  
This header provides functions for dynamic memory management. 

* [[env/inline_mem.h]]  
This header provides APIs for inline memory copying and filling functions.

* [[env/mutex.h]]  
This header provides APIs for [mutexes](https://en.wikipedia.org/wiki/Mutex).

* [[env/once_flag.h]]  
This header provides APIs for once-initialization. These functions make up a much more powerful interface than `pthread_once()`.

* [[env/pp.h]]  
This header provides macros for playing with the C preprocessor.

* [[env/thread.h]]  
This header provides APIs for [threads](https://en.wikipedia.org/wiki/Thread_(computing)).

* [[env/tls.h]]  
This header provides APIs for [thread local storage(TLS)](https://en.wikipedia.org/wiki/Thread-local_storage) and callbacks on thread exit.

* [[env/xassert.h]]  
This header provides macros similar to the standard `assert()` macro with a few improvements.

* [[ext/itow.h]]  
This header provides functions that convert integers to wide strings.

* [[ext/wcpcpy.h]]  
This header provides functions that copy or concatenate wide strings like the `wcpcpy()` function in POSIX-2008.

* [[c11thread.h]]  
This header is a shortcut for [[env/c11thread.h]].

* [[gthread.h]]  
This header is a shortcut for [[env/gthread.h]].

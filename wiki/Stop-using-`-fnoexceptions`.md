# Stop using `-fno-exceptions`

## What People Must Know Before Using Exceptions

It is common for programmers to make use of exceptions. I am not going to talk about the benefit of exceptions, which is apparently off-topic for this article.

Before you use exceptions in your code there is something very important you must know, _exception safety_.

Basically, there are three levels of exception safety:

### _No-throw guarantee_

**This operation does not throw an exception.**

A notable example is `::operator delete()`. In addition, destructors do not throw exceptions most of the times. In particular, no destructors from the standard library throw exceptions.

### _Strong exception safety guarantee_

**If this operation throws an exception, there is no effect.**

This is usually done in two ways:
 
1) Rollback the effect upon an exception via some no-throw mechanism, then re-throw the exception. This is known as _commit or rollback_. The following code makes two accounts friends of each other, using _commit or rollback_:

    ```c++
    std::multimap<std::string, std::string> friend_map;
    
    void make_friends(std::string account1, std::string account2){
        const auto result1 = friend_map.emplace(account1, account2);
        try {
            friend_map.emplace(std::move(account2), std::move(account1));
        } catch(...){
            // Undo the first emplace and re-throw the exception.
            friend_map.erase(result1.first);
            throw;
        }
    }
    ```

2) Copy the to-be-modified objects, modify them, then overwrite all old ones via some no-throw mechanism (in C++98 days this was usually done via `swap()`). This is known as _copy and swap_. Copying an object might be a heavyweight operation, but from a programmer's point of view this is a simple, crude but effective solution, as it is no longer needed to rollback the side effects explicitly. The following code does the same thing with the above example, using _copy and swap_:

    ```c++
    std::multimap<std::string, std::string> friend_map;
    
    void make_friends(std::string account1, std::string account2){
        auto temp_map = friend_map;
        temp_map.emplace(account1, account2);
        temp_map.emplace(std::move(account2), std::move(account1));
        friend_map = std::move(temp_map); // Alternatively: friend_map.swap(temp_map);
    }
    ```

### _Basic exception safety guarantee_

**If this operation throws an exception, all invariants are preserved.**

That is to say, there is no resource leak and all objects are left in valid but unspecified states. Implicitly defined copy assignment operators usually have only _basic exception safety guarantee_, as shown in the following code:

```c++
struct foo {
    std::string name;
    std::string desc;
    
    // This is identical to the implicitly defined copy assignment operator.
    // Basic exception safety guarantee.
    foo &operator=(const foo &rhs){
        name = rhs.name;
        desc = rhs.desc; // What if an exception is thrown here?
        return *this;
    }
    // This is identical to the implicitly defined move assignment operator.
    // No-throw guarantee.
    foo &operator=(foo &&rhs) noexcept {
        name = std::move(rhs.name);
        desc = std::move(rhs.desc); // Throws nothing.
        return *this;
    }
};
```

## Why People Want `-fno-exceptions` and What People Get

It easily comes to people's minds that they can always use exceptions to simplify their work. For example, one may wrap [CreateWindowEx()](https://msdn.microsoft.com/en-us/library/windows/desktop/ms632680(v=vs.85).aspx) with a _try-block_ and wish to catch exceptions thrown in the [WindowProc](https://msdn.microsoft.com/en-us/library/windows/desktop/ms633573(v=vs.85).aspx). It might 'work', but, nonetheless, is not guaranteed to work. I don't know what language it was that Microsoft used to write the window APIs, but let's assume it was C. Look at the following example:

```c
SRWLOCK mutex = SRWLOCK_INIT;
struct shared_data data = { 0 };

void process(void func(struct shared_data *)){
    AcquireSRWLockExclusive(&mutex);
    func(&data);
    ReleaseSRWLockExclusive(&mutex);
}
```

Saying it is C, I wouldn't say it contains a bug, and never will any C programmer say so. However, what if `func(&data)` throws an exception? Apparently, `ReleaseSRWLockExclusive()` will never be called, leaving `mutex` in a locked state forever. How can 'bug-free' code be broken? It is 'bug-free' unless we take exceptions into account, while C actually does not have exceptions, and in this example there is no standard way to handle exceptions.

There is only one solution: Never throw exceptions from foreign ('foreign' as in 'foreign languages') callbacks, which however, isn't _practically_ possible. Quite a little construction of the standard library depends on exceptions. For example, `std::vector::resize()` could end up with an instance of `std::bad_alloc` thrown, `dynamic_cast`'ing a reference to a base class to a reference to its derived class could end up with an instance of `std::bad_cast` thrown, etc. Any code that is expected to be called from a context of an unknown language must catch exceptions as needed before the control flow is taken over or returned to that context, which could be messy.

Some smarties turn to `-fno-exceptions`, which guarantees all their code the _no-throw guarantee_ effectively. The compiler makes static checks to ensure there is no explicit `throw` expression. Yes, any operation that would have thrown an exception now results in a call to `std::abort()` instead, and there is no need to catch exceptions because there _could not_ be exceptions.

Please consider this ostrichism harmful:

1. You would have to write a number of `if` statements to deal with unexpected results, which are 99% unnecessary if you follow even the _basic exception safety guarantee_. And if you carelessly forget an `if` you may get something dangerous, for example, calling `std::vector::operator[]()` with an out-of-range index results in undefined behavior such as memory corruption, but `std::vector::at()` would throw an exception instead. If your code has sort of top exception handlers you can log the exception and do some recovery such as disconnecting the client that initiates the request in question, without crashing the entire server process.  
2. You can't use most of the standard containers (except `std::array` of course), unless you shut your eyes when stealing cream. Dynamic memory allocation could fail and result in `std::bad_alloc`.  
3. `-fno-exceptions` doesn't result in exception-safe code. It merely results in code with _no-throw guarantee_. Exception-unsafe code, for example, code written in C, can be called from exception-safe code with little modification. On the contrary, exception-safe code that doesn't have _no-throw guarantee_ can't be called from exception-unsafe code without modification. Practically, if a C++ function is to be called from a C function through some callback mechanism, the C++ function must catch any exception thrown from it and return some error codes to the C function instead. The use of `-fno-exceptions` seals the termination upon exceptions, essentially infecting all users of that code, preventing them from handling the bad condition (for example, `std::bad_alloc`) even when they could, which could not be corrected without refactoring the exception-unsafe code in question.

## No `-fno-exceptions` Anymore

Here are my practical suggestions:

1. Don't pass callbacks that may throw exceptions to foreign code, because you can't assume it can handle exceptions.  
2. Don't assume callbacks passed from foreign code may throw exceptions, because you can't handle them, either.  

These two rules renders `-fno-exceptions` useless effectively. Why `-fno-exceptions` anymore? :joy:

# Update \[2017-01-02\]

I noticed that someone had posted this article onto [reddit](https://www.reddit.com/r/cpp/comments/5l1pfp/stop_using_fnoexceptions/) and there were [some arguments](http://seanmiddleditch.com/keep-disabling-exceptions/) by _Sean Middleditch_ about this.

Well, this article is here mostly because someone tried to compile mcfgthread with `-fexceptions` (yes mcfgthread is basically in C and doesn't use C++ exceptions) and got some errors. This article is NOT a tutorial and is NOT suitable for beginners, as it doesn't teach people how to deal with exceptions. It merely expresses my criticism about the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) which is for experienced C++ programmers thus is extremely misleading. Nevertheless, he was right that this article was off-topic from its title _a lot_. I have fixed it up.

Here I would like to say something about _Sean Middleditch_'s opinion:

> ### Impossible strong exception safety guarantee
> `flat_set` for instance is a very simple container that cannot be left in a valid state on exception, except by dropping all of its contents. If an exception is thrown while shuffling values around, the container must either be left with a hole (breaking the ordering invariant) or left with a duplicate (breaking the unique-keys invariant).

Most of the times this isn't true. A brutal deep copy-and-swap could almost always provide strong exception safety.

It would indeed be a problem if the move constructor of `value_type` could throw and either:

1. the copy constructor is deleted, where you can't _copy_ safely, or  
2. the storage of elements isn't dynamic, like `static_vector`, where you can't _swap_ safely.  

Apparently both happen only if the move constructor throws, no? We can `static_assert` it out and make such code fail at compile time rather than run time.

> Let's not even get started on what led to C++17's variant and its valueless_by_exception state.

You are right. Nevertheless I vote for such a state as it guarantees `variant` a default constructor that never throws, which is otherwise impossible.

> ### Basic exception safety guarantee and data loss
> If this happens and the user then saves their document/game/whatever, they just lost their data.

This is laughable. If your editor/game/whatever could have thrown an exception but it couldn't because of `-fno-exceptions`, it merely crashes. No data could be recovered either.

> ### Foreign function interfaces at any level
> Code authors shouldn't need to know if their code is called in such a context, because otherwise that means their code isn't composable.

Correct. Then such authors _shall not_ expect the callers can pass and handle C++ exceptions in such contexts. They must catch exceptions (again, don't `catch(...)` unless you are going to call `abort()` here) before the control flow is taken over.

> If the author of the foreign callback is just expecting to catch all exceptions, how do they know which exceptions will be thrown? All they can do is catch(...) and pray there's something meaningfully correct to do in that case (hopefully some kind of general "everything failed" callback result).

Who needs such a result? It sounds like `switch(errno){ /* ... */ default: return WHAT_THE_HECK_ERROR; }` and it isn't exception that is to blame.

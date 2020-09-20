# env/bail.h

## Macros

None.

## Types

None.

## Functions

#### `noreturn void _MCFCRT_Bail(const wchar_t *pwszDescription);`

* **Description**

    This function displays a message box with the specified text and causes the current process to exit abnormally once the message box is closed. The contents in the message box are also copied into the standard error stream if possible.  

    If the macro `NDEBUG` is defined when this library is compiled or `IsDebuggerPresent()` returns a non-zero value, the user is shown a message box with an 'OK' button and a 'Cancel' button. Otherwise, the user is shown a message box with an 'OK' button only.  
    If the user clicks the 'OK' button, the process is terminated immediately. If the user clicks the 'Cancel' button, a breakpoint is triggered, enabling a debugger to catch the breakpoint and view the stack backtrace, if any.  
    It is possible for multiple threads to call this function simultaneously. The first thread that calls this function displays the message box. All other threads terminate themselves with `TerminateThread()`.

* **Parameters**

    `const wchar_t *pwszDescription`  
    The message to display. This parameter must not be `NULL`.

* **Return Value**

    This function cannot return.

* **Remarks**

    This function uses only the undocumented `NtRaiseHardError()` system call from `NTDLL.DLL` and a few I/O functions from `KERNEL32.DLL` without any other CRT functions.  
    Hence it is safe to call this function inside TLS callbacks and the `DllMain()` function.  

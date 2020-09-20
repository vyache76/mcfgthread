# env/crt_module.h

## Macros

None.

## Types

#### `typedef void (*_MCFCRT_AtCrtModuleExitCallback)(intptr_t nContext);`

* **Description**

    This is the type for callbacks that can be registered to be invoked upon the current program's exit.

## Functions

#### `bool _MCFCRT_AtCrtModuleQuickExit(_MCFCRT_AtCrtModuleExitCallback pfnProc, intptr_t nContext);`

* **Description**

    This function registers a callback to be invoked upon the current program's exit if `_MCFCRT_QuickExit()` is called.  
    The callbacks are invoked in a first in, last out order.

* **Parameters**

    `_MCFCRT_AtCrtModuleExitCallback pfnProc`  
    The callback to be invoked.

    `intptr_t nContext`  
    Parameter for the callback.

* **Return Value**

    This function returns `true` on success and `false` on failure. To get extended error information, call `GetLastError()`.

* **Remarks**

    It is possible to call this function in other quick exit callbacks. Callbacks registered in such scenarios are guaranteed to be invoked in a first in, last out order, before any other existent callbacks.

#### `noreturn void _MCFCRT_ImmediateExit(unsigned uExitCode);`

* **Description**

    This function terminates the current process immediately, as if by calling `TerminateProcess(GetCurrentProcess(), uExitCode)`. No destructors or callbacks are called, nor do any DLLs loaded receive a `DLL_PROCESS_DETACH` notification.

* **Parameters**

    `unsigned uExitCode`  
    The process exit code passed to the operating system.

* **Return Value**

    This function cannot return.

#### `noreturn void _MCFCRT_QuickExit(unsigned uExitCode);`

* **Description**

    This function terminates the current process quickly. All callbacks registered with `_MCFCRT_AtCrtModuleQuickExit()` are called in the reverse order in which they have been registered. The process is then terminated, as if by calling `_MCFCRT_ImmediateExit(uExitCode)`.

* **Parameters**

    `unsigned uExitCode`  
    The process exit code passed to the operating system.

* **Return Value**

    This function cannot return.

#### `noreturn void _MCFCRT_Exit(unsigned uExitCode);`

* **Description**

    This function terminates the current process gracefully, as if by calling `ExitProcess(uExitCode)`. All DLLs loaded except `KERNEL32.DLL` and `NTDLL.DLL` are sent a `DLL_PROCESS_DETACH` notification. Callbacks registered with `_MCFCRT_AtCrtModuleQuickExit()` are discarded. The process is terminated thereafter.

* **Parameters**

    `unsigned uExitCode`  
    The process exit code passed to the operating system.

* **Return Value**

    This function cannot return.

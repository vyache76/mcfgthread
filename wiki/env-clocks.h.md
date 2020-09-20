# env/clocks.h

## Macros

None.

## Types

None.

## Functions

#### `uint32_t _MCFCRT_ReadTimeStampCounter32(void);`
#### `uint64_t _MCFCRT_ReadTimeStampCounter64(void);`

* **Description**

    Both functions return the [time stamp counter](https://en.wikipedia.org/wiki/Time_Stamp_Counter) of the current CPU core, suitable for random seeds.  
    The time stamp counter is a 64-bit unsigned integer. If the value cannot be represented in the result type, its higher part is discarded.

* **Parameters**

    Both functions take no parameters.

* **Return Value**

    The return value is the (possibly truncated) value of the timestamp counter.  

* **Remarks**

    Either function is equivalent to `__builtin_ia32_rdtsc()` with the result casted to the corresponding type.

#### `uint64_t _MCFCRT_GetUtcClock(void);`
#### `uint64_t _MCFCRT_GetLocalClock(void);`

* **Description**

    Both functions return the number of milliseconds since `1970-01-01 00:00:00` in UTC or the local timezone, respectively.

* **Parameters**

    Both functions take no parameters.

* **Return Value**

    The return value is the number of milliseconds since `1970-01-01 00:00:00` in the specified timezone.

* **Remarks**

    The result of the former in seconds (divided by 1000) is more commonly known as the [UNIX time](https://en.wikipedia.org/wiki/Unix_time).

#### `uint64_t _MCFCRT_GetUtcClockFromLocal(uint64_t u64LocalClock);`

* **Description**

    This function converts the specified local time to UTC time.  
    It performs the reverse operation of `_MCFCRT_GetLocalClockFromUtc()`.

* **Parameters**

    `uint64_t u64LocalClock`  
    The local time to convert, represented as the number of milliseconds since `1970-01-01 00:00:00` in the local timezone.

* **Return Value**

    The return value is the number of milliseconds since `1970-01-01 00:00:00` in UTC.

* **Remarks**

    `_MCFCRT_GetUtcClockFromLocal(_MCFCRT_GetLocalTime())` is roughly equivalent to `_MCFCRT_GetUtcTime()`.

#### `uint64_t _MCFCRT_GetLocalClockFromUtc(uint64_t u64UtcClock);`

* **Description**

    This function converts the specified UTC time to local time.  
    It performs the reverse operation of `_MCFCRT_GetUtcClockFromLocal()`.

* **Parameters**

    `uint64_t u64UtcClock`  
    The UTC time to convert, represented as the number of milliseconds since `1970-01-01 00:00:00` in UTC.

* **Return Value**

    The return value is the number of milliseconds since `1970-01-01 00:00:00` in the local timezone.

* **Remarks**

    `_MCFCRT_GetLocalClockFromUtc(_MCFCRT_GetUtcTime())` is roughly equivalent to `_MCFCRT_GetLocalTime()`.

#### `uint64_t _MCFCRT_GetFastMonoClock(void);`
#### `double _MCFCRT_GetHiResMonoClock(void);`

* **Description**

    Either function returns the value of a distinct and otherwise unspecified monotonic clock.  
    A monotonic clock can't be set. Its value goes straight forward once the system starts up.  
    The absolute value of a monotonic clock doesn't mean anything; only the difference of a pair of values of it does.  
    Monotonic clocks are widely used for time measure purposes.

* **Parameters**

    Both functions take no parameters.

* **Return value**

    The return value is the value of the specified monotonic clock.  
    These clocks are NOT interchangeable. DO NOT subtract the value of one clock from that of the other.

* **Remarks**

    `_MCFCRT_GetFastMonoClock()` is implemented using `GetTickCount64()` and `_MCFCRT_GetHiResMonoClock()` is implemented using `QueryPerformanceCounter()`.

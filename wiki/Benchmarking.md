# Benchmarking

## Environment

1. **CPU**: Intel Xeon E3-1230 v3 @ 3.30GHz (8 logical cores)
2. **RAM**: ADATA DDR3 16 GiB @ 1600MHz
3. **OS**: Windows 7 Professional SP1 x64
4. **Compiler**: [gcc (gcc-7-branch HEAD with MCF thread model, built by LH_Mouse.) 7.2.1 20180104](https://gcc-mcf.lhmouse.com/)

## Test Program (C++)

```c++
// g++ ping_pong.cpp -std=c++11 -O2 -DN_THREADS=4 -DN_LOOPS=2000000 -DTHREAD_MODEL_MCF

#define _WIN32_WINNT 0x0601
#include <windows.h>

#include <iostream>
#include <thread>
#include <array>
#include <atomic>
#include <cstdlib>
#include <cstddef>

#if THREAD_MODEL_NATIVE
#   define MUTEX_T           SRWLOCK
#   define MUTEX_INIT        SRWLOCK_INIT
#   define MUTEX_LOCK(m)     AcquireSRWLockExclusive(m)
#   define MUTEX_UNLOCK(m)   ReleaseSRWLockExclusive(m)
#   define COND_T            CONDITION_VARIABLE
#   define COND_INIT         CONDITION_VARIABLE_INIT
#   define COND_WAIT(cv,m)   SleepConditionVariableSRW(cv,m,INFINITE,0)
#   define COND_SIGNAL(cv)   WakeConditionVariable(cv)
#elif THREAD_MODEL_POSIX
#   include <pthread.h>
#   define MUTEX_T           pthread_mutex_t
#   define MUTEX_INIT        PTHREAD_MUTEX_INITIALIZER
#   define MUTEX_LOCK(m)     pthread_mutex_lock(m)
#   define MUTEX_UNLOCK(m)   pthread_mutex_unlock(m)
#   define COND_T            pthread_cond_t
#   define COND_INIT         PTHREAD_COND_INITIALIZER
#   define COND_WAIT(cv,m)   pthread_cond_wait(cv,m)
#   define COND_SIGNAL(cv)   pthread_cond_signal(cv)
#elif THREAD_MODEL_MCF
#   define MUTEX_T           __gthread_mutex_t
#   define MUTEX_INIT        __GTHREAD_MUTEX_INIT
#   define MUTEX_LOCK(m)     __MCFCRT_gthread_mutex_lock(m)
#   define MUTEX_UNLOCK(m)   __MCFCRT_gthread_mutex_unlock(m)
#   define COND_T            __gthread_cond_t
#   define COND_INIT         __GTHREAD_COND_INIT
#   define COND_WAIT(cv,m)   __MCFCRT_gthread_cond_wait(cv,m)
#   define COND_SIGNAL(cv)   __MCFCRT_gthread_cond_signal(cv)
#else
#   error You must specify a thread model.
#endif

class semaphore {
private:
    MUTEX_T x_m;
    COND_T x_cv;
    std::size_t x_n;

public:
    explicit semaphore(std::size_t n_init = 0)
        : x_m(MUTEX_INIT), x_cv(COND_INIT), x_n(n_init)
    { }

public:
    void p(){
        MUTEX_LOCK(&x_m);
        while(x_n == 0){
            COND_WAIT(&x_cv, &x_m);
        }
        --x_n;
        MUTEX_UNLOCK(&x_m);
    }
    void v(){
        MUTEX_LOCK(&x_m);
        if(x_n == static_cast<std::size_t>(-1)){
            std::abort();
        }
        ++x_n;
        COND_SIGNAL(&x_cv);
        MUTEX_UNLOCK(&x_m);
    }
};

int main(){
    constexpr std::size_t n_threads = N_THREADS;
    constexpr std::size_t n_loops = N_LOOPS;

    std::atomic<std::size_t> n_ping(0), n_pong(0);

    std::array<std::thread, n_threads> threads;
    semaphore sem_g;
    std::array<semaphore, n_threads> sems;
    for(std::size_t i = 0; i < n_threads; ++i){
        threads.at(i) = std::thread([&,i]{
            for(std::size_t j = 0; j < n_loops; ++j){
                sems.at(i).p();
                n_pong.fetch_add(1, std::memory_order_relaxed);
                sem_g.v();
            }
        });
    }
    const auto t1 = std::chrono::high_resolution_clock::now();
    for(std::size_t j = 0; j < n_loops; ++j){
        for(std::size_t i = 0; i < n_threads; ++i){
            n_ping.fetch_add(1, std::memory_order_relaxed);
            sems.at(i).v();
        }
        for(std::size_t i = 0; i < n_threads; ++i){
            sem_g.p();
        }
    }
    const auto t2 = std::chrono::high_resolution_clock::now();
    for(std::size_t i = 0; i < n_threads; ++i){
        threads.at(i).join();
    }

    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout <<"n_ping = " <<n_ping.load(std::memory_order_relaxed) <<std::endl
              <<"n_pong = " <<n_pong.load(std::memory_order_relaxed) <<std::endl
              <<"duration = " <<duration.count() <<" ms" <<std::endl;
}
```

## Test Runner (Shell)

```sh
#!/bin/sh

set -e

function test_7(){
    g++ ping_pong.cpp -std=c++11 -O2 -Wl,-s \
        -Irelease/mingw64/include -Lrelease/mingw64/lib -lwinpthread \
        -DN_THREADS=$1 -DN_LOOPS=$2  -DTHREAD_MODEL_$3 -o ping_pong.exe
    _str=''
    for ((_i=1;_i<=7;++_i)); do
       echo -ne '\r' $_i
       _str+=' '
       _str+=$(./ping_pong.exe | sed -r 's/^duration = ([0-9]+) ms$/\1/;t;d')
    done
    echo -ne '\r' $_str '\n'
}

test_7  1 3200000 MCF
test_7  1 3200000 POSIX
test_7  1 3200000 NATIVE

test_7  2 1600000 MCF
test_7  2 1600000 POSIX
test_7  2 1600000 NATIVE

test_7  4  800000 MCF
test_7  4  800000 POSIX
test_7  4  800000 NATIVE

test_7  8  400000 MCF
test_7  8  400000 POSIX
test_7  8  400000 NATIVE

test_7 16  200000 MCF
test_7 16  200000 POSIX
test_7 16  200000 NATIVE

test_7 32  100000 MCF
test_7 32  100000 POSIX
test_7 32  100000 NATIVE
```

## Test Results

* **Threads**: Number of threads to create.

    This is specified using the option `-DN_THREADS=`.

* **Loops**: Number of loops per thread.

    This is specified using the option `-DN_LOOPS=`.

* **Model**: Which thread library to use.

    We only want mutexes and condition variables here. This is specified using one of the following options:

    1. `-DTHREAD_MODEL_MCF`:

         Use `__MCFCRT_gthread_*` ones from _mcfgthread_.

    2. `-DTHREAD_MODEL_POSIX -pthread`:

         Use `pthread_*` ones from _winpthreads_.

    3. `-DTHREAD_MODEL_NATIVE`:

         Use native ones from _KERNEL32_.

* **1st - 7th**: Number of milliseconds of each test round.

* **Fastest**, **Slowest**: The minimum and maximums numbers of milliseconds of all the seven test rounds.

* **Average**: Average number of milliseconds of the five test rounds, excluding both the minimum and maximum ones.

#### Test Results of x64

|Threads|Loops|Model|1st|2nd|3rd|4th|5th|6th|7th|Threads|Loops|Model|Fastest|Slowest|Average|
|------:|----:|:----|--:|--:|--:|--:|--:|--:|--:|------:|----:|:----|------:|------:|------:|
|1|3200000|MCF|1289|1216|1285|1233|1267|1247|1281|1|3200000|MCF|1216|1289|1262.6|
|||POSIX|14110|15452|16943|13869|14400|13567|14695|||POSIX|13567|16943|14505.2|
|||NATIVE|1699|1659|1706|1695|1698|1674|1691|||NATIVE|1659|1706|1691.4|
|2|1600000|MCF|1548|1493|1517|1483|1555|1452|1451|2|1600000|MCF|1451|1555|1498.6|
|||POSIX|8996|6951|6782|7484|8354|6726|8328|||POSIX|6726|8996|7579.8|
|||NATIVE|2684|2651|2622|2704|2607|2593|2540|||NATIVE|2540|2704|2631.4|
|4|800000|MCF|1440|1480|1393|1440|1385|1386|1406|4|800000|MCF|1385|1480|1413.0|
|||POSIX|5927|9659|7866|5650|5999|5834|6291|||POSIX|5650|9659|6383.4|
|||NATIVE|1685|1657|1688|1707|1704|1676|1685|||NATIVE|1657|1707|1687.6|
|8|400000|MCF|2778|2739|2743|2774|2732|2713|2696|8|400000|MCF|2696|2778|2740.2|
|||POSIX|4296|4124|4153|4267|4289|4251|4742|||POSIX|4124|4742|4251.2|
|||NATIVE|3814|3783|3797|3811|3785|3801|3771|||NATIVE|3771|3814|3795.4|
|16|200000|MCF|2487|2463|2412|2426|2419|2387|2401|16|200000|MCF|2387|2487|2424.2|
|||POSIX|3363|3399|3363|3518|3473|3335|3371|||POSIX|3335|3518|3393.8|
|||NATIVE|3568|3556|3566|3581|3572|3549|3526|||NATIVE|3526|3581|3562.2|
|32|100000|MCF|2303|2291|2272|2309|2327|2327|2350|32|100000|MCF|2272|2350|2311.4|
|||POSIX|2982|2913|2930|2923|2934|2893|2956|||POSIX|2893|2982|2931.2|
|||NATIVE|3447|3409|3409|3388|3402|3626|3664|||NATIVE|3388|3664|3458.6|

#### Test Results of x86

|Threads|Loops|Model|1st|2nd|3rd|4th|5th|6th|7th|Threads|Loops|Model|Fastest|Slowest|Average|
|------:|----:|:----|--:|--:|--:|--:|--:|--:|--:|------:|----:|:----|------:|------:|------:|
|1|3200000|MCF|1095|1030|1079|1031|1118|1138|1085|1|3200000|MCF|1030|1138|1081.6|
|||POSIX|16034|14585|17398|16581|14865|15886|15491|||POSIX|14585|17398|15771.4|
|||NATIVE|1669|1682|1735|1648|1717|1620|1681|||NATIVE|1620|1735|1679.4|
|2|1600000|MCF|1492|1456|1448|1457|1451|1442|1441|2|1600000|MCF|1441|1492|1450.8|
|||POSIX|8875|8130|7321|8832|8585|7297|8005|||POSIX|7297|8875|8174.6|
|||NATIVE|2632|2625|2615|2581|2611|2581|2620|||NATIVE|2581|2632|2610.4|
|4|800000|MCF|1452|1355|1398|1375|1468|1489|1462|4|800000|MCF|1355|1489|1431.0|
|||POSIX|6629|6446|9804|8001|5812|6316|6296|||POSIX|5812|9804|6737.6|
|||NATIVE|1629|1641|1635|1603|1646|1599|1601|||NATIVE|1599|1646|1621.8|
|8|400000|MCF|3169|3137|3154|3148|3153|3150|3126|8|400000|MCF|3126|3169|3148.4|
|||POSIX|4747|4911|4634|4650|4652|4446|4502|||POSIX|4446|4911|4637.0|
|||NATIVE|4193|4167|4171|4159|4166|4171|4179|||NATIVE|4159|4193|4170.8|
|16|200000|MCF|2870|2831|2838|2836|2826|2780|2774|16|200000|MCF|2774|2870|2822.2|
|||POSIX|3918|3956|3783|3821|3772|3874|3943|||POSIX|3772|3956|3867.8|
|||NATIVE|3892|3820|3803|3858|3892|3804|3806|||NATIVE|3803|3892|3836.0|
|32|100000|MCF|2738|2742|2762|2717|2710|2776|2769|32|100000|MCF|2710|2776|2745.6|
|||POSIX|3422|3357|3313|3367|3359|3363|3274|||POSIX|3274|3422|3351.8|
|||NATIVE|3718|3663|3678|3667|3737|3731|3711|||NATIVE|3663|3737|3701.0|

## One More Test for Timed Waiting

The following patch makes the program above perform timed waiting:

```diff
--- ping_pong.cpp.orig	2018-01-05 14:27:56.680413200 +0800
+++ ping_pong.cpp	2018-01-05 14:27:16.917138900 +0800
@@ -10,6 +10,8 @@
 #include <cstdlib>
 #include <cstddef>
 
+::timespec infinite = { 2000000000, 0 };
+
 #if THREAD_MODEL_NATIVE
 #   define MUTEX_T           SRWLOCK
 #   define MUTEX_INIT        SRWLOCK_INIT
@@ -17,7 +19,7 @@
 #   define MUTEX_UNLOCK(m)   ReleaseSRWLockExclusive(m)
 #   define COND_T            CONDITION_VARIABLE
 #   define COND_INIT         CONDITION_VARIABLE_INIT
-#   define COND_WAIT(cv,m)   SleepConditionVariableSRW(cv,m,INFINITE,0)
+#   define COND_WAIT(cv,m)   SleepConditionVariableSRW(cv,m,100000000,0)
 #   define COND_SIGNAL(cv)   WakeConditionVariable(cv)
 #elif THREAD_MODEL_POSIX
 #   include <pthread.h>
@@ -27,7 +29,7 @@
 #   define MUTEX_UNLOCK(m)   pthread_mutex_unlock(m)
 #   define COND_T            pthread_cond_t
 #   define COND_INIT         PTHREAD_COND_INITIALIZER
-#   define COND_WAIT(cv,m)   pthread_cond_wait(cv,m)
+#   define COND_WAIT(cv,m)   pthread_cond_timedwait(cv,m,&infinite)
 #   define COND_SIGNAL(cv)   pthread_cond_signal(cv)
 #elif THREAD_MODEL_MCF
 #   define MUTEX_T           __gthread_mutex_t
@@ -36,7 +38,7 @@
 #   define MUTEX_UNLOCK(m)   __MCFCRT_gthread_mutex_unlock(m)
 #   define COND_T            __gthread_cond_t
 #   define COND_INIT         __GTHREAD_COND_INIT
-#   define COND_WAIT(cv,m)   __MCFCRT_gthread_cond_wait(cv,m)
+#   define COND_WAIT(cv,m)   __MCFCRT_gthread_cond_timedwait(cv,m,&infinite)
 #   define COND_SIGNAL(cv)   __MCFCRT_gthread_cond_signal(cv)
 #else
 #   error You must specify a thread model.
```

We set timeouts to huge values so it is effectively impossible to time out. For your reference, the `std::condition_variable` from MSVC 2015 behaves poorly when a timeout is given.

## Test Results of Timed Waiting

#### Test Results of x64

|Threads|Loops|Model|1st|2nd|3rd|4th|5th|6th|7th|Threads|Loops|Model|Fastest|Slowest|Average|
|------:|----:|:----|--:|--:|--:|--:|--:|--:|--:|------:|----:|:----|------:|------:|------:|
|1|3200000|MCF|1356|1332|1230|1326|1381|1345|1346|1|3200000|MCF|1230|1381|1341.0|
|||POSIX|13599|13833|14356|13459|13765|13663|13284|||POSIX|13284|14356|13663.8|
|||NATIVE|1590|1631|1543|1603|1598|1647|1507|||NATIVE|1507|1647|1593.0|
|2|1600000|MCF|1540|1482|1511|1547|1506|1527|1481|2|1600000|MCF|1481|1547|1513.2|
|||POSIX|7580|7770|8199|13421|10916|8533|10229|||POSIX|7580|13421|9129.4|
|||NATIVE|2690|2658|2777|2709|2679|2757|2673|||NATIVE|2658|2777|2701.6|
|4|800000|MCF|1390|1365|1308|1402|1384|1451|1362|4|800000|MCF|1308|1451|1380.6|
|||POSIX|6145|6841|6040|6181|6707|6013|6529|||POSIX|6013|6841|6320.4|
|||NATIVE|1750|1694|1671|1784|1630|1720|1686|||NATIVE|1630|1784|1704.2|
|8|400000|MCF|2629|2702|2698|2670|2631|2656|2710|8|400000|MCF|2629|2710|2671.4|
|||POSIX|4585|4150|5101|4067|4311|3973|5180|||POSIX|3973|5180|4442.8|
|||NATIVE|3663|3912|3811|3666|3906|3916|3925|||NATIVE|3663|3925|3842.2|
|16|200000|MCF|2507|2480|2469|2449|2499|2470|2466|16|200000|MCF|2449|2507|2476.8|
|||POSIX|3488|3497|3470|3387|3444|3476|3495|||POSIX|3387|3497|3474.6|
|||NATIVE|3596|3514|3502|3463|3489|3497|3426|||NATIVE|3426|3596|3493.0|
|32|100000|MCF|2275|2372|2377|2327|2351|2308|2302|32|100000|MCF|2275|2377|2332.0|
|||POSIX|2961|2944|2891|2967|2918|2934|2904|||POSIX|2891|2967|2932.2|
|||NATIVE|3524|3507|3506|3455|3491|3515|3485|||NATIVE|3455|3524|3500.8|

#### Test Results of x86

|Threads|Loops|Model|1st|2nd|3rd|4th|5th|6th|7th|Threads|Loops|Model|Fastest|Slowest|Average|
|------:|----:|:----|--:|--:|--:|--:|--:|--:|--:|------:|----:|:----|------:|------:|------:|
|1|3200000|MCF|1592|1622|1635|1732|1661|1642|1561|1|3200000|MCF|1561|1732|1630.4|
|||POSIX|14556|15183|15778|14734|14814|14471|15312|||POSIX|14471|15778|14919.8|
|||NATIVE|1666|1698|1683|1677|1644|1664|1675|||NATIVE|1644|1698|1673.0|
|2|1600000|MCF|1676|1622|1617|1616|1650|1676|1635|2|1600000|MCF|1616|1676|1640.0|
|||POSIX|11150|8675|8144|7539|7434|12013|8095|||POSIX|7434|12013|8720.6|
|||NATIVE|2635|2612|2656|2582|2612|2620|2674|||NATIVE|2582|2674|2627.0|
|4|800000|MCF|1625|1543|1646|1504|1507|1575|1519|4|800000|MCF|1504|1646|1553.8|
|||POSIX|6681|6669|6317|6663|6536|6879|6513|||POSIX|6317|6879|6612.4|
|||NATIVE|1661|1608|1629|1651|1644|1671|1634|||NATIVE|1608|1671|1643.8|
|8|400000|MCF|3164|3196|3158|3167|3155|3186|3154|8|400000|MCF|3154|3196|3166.0|
|||POSIX|5030|4699|4746|4680|4700|4545|4723|||POSIX|4545|5030|4709.6|
|||NATIVE|4319|4282|4270|4271|4274|4283|4278|||NATIVE|4270|4319|4277.6|
|16|200000|MCF|2955|2879|2915|2924|2864|2853|2898|16|200000|MCF|2853|2955|2896.0|
|||POSIX|3907|3891|3908|3884|3891|3846|3884|||POSIX|3846|3908|3891.4|
|||NATIVE|3983|3925|3936|3950|3932|3940|3931|||NATIVE|3925|3983|3937.8|
|32|100000|MCF|2797|2840|2779|2766|2797|2780|2778|32|100000|MCF|2766|2840|2786.2|
|||POSIX|3342|3302|3392|3375|3356|3328|3365|||POSIX|3302|3392|3353.2|
|||NATIVE|3826|3756|3795|3807|3800|3792|3814|||NATIVE|3756|3826|3801.6|

## Conclusion

1. Mutexes and condition variables from _mcfgthread_ prevail thanks to [heuristic contention detection](https://github.com/lhmouse/mcfgthread/wiki/Pseudo-Code).

2. Mutexes and condition variables from _winpthreads_ are not very efficient. They are surprisingly slow when there is little contention. The advantage of _winpthreads_ is that it provides POSIX compatibility.

3. Windows native SRW locks and condition variables have reasonably good performance in all cases. However SRW locks do not support timed waiting. Despite the feasibility, it is not very efficient to implement an `std::timed_mutex` using an SRW lock + a condition variable.

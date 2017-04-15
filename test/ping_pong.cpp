// This file is put into the Public Domain.

#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <pthread.h>

#include <iostream>
#include <thread>
#include <array>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <atomic>
#include <cstddef>

#if defined(TEST_NATIVE) && (TEST_NATIVE)
#	define MUTEX_T           SRWLOCK
#	define MUTEX_INIT        SRWLOCK_INIT
#	define MUTEX_LOCK(m)     AcquireSRWLockExclusive(m)
#	define MUTEX_UNLOCK(m)   ReleaseSRWLockExclusive(m)
#	define COND_T            CONDITION_VARIABLE
#	define COND_INIT         CONDITION_VARIABLE_INIT
#	define COND_WAIT(cv,m)   SleepConditionVariableSRW(cv,m,INFINITE,0)
#	define COND_SIGNAL(cv)   WakeConditionVariable(cv)
#elif defined(TEST_PTHREAD) && (TEST_PTHREAD)
#	define MUTEX_T           pthread_mutex_t
#	define MUTEX_INIT        PTHREAD_MUTEX_INITIALIZER
#	define MUTEX_LOCK(m)     pthread_mutex_lock(m)
#	define MUTEX_UNLOCK(m)   pthread_mutex_unlock(m)
#	define COND_T            pthread_cond_t
#	define COND_INIT         PTHREAD_COND_INITIALIZER
#	define COND_WAIT(cv,m)   pthread_cond_wait(cv,m)
#	define COND_SIGNAL(cv)   pthread_cond_signal(cv)
#else
#	define MUTEX_T           __gthread_mutex_t
#	define MUTEX_INIT        __GTHREAD_MUTEX_INIT
#	define MUTEX_LOCK(m)     __gthread_mutex_lock(m)
#	define MUTEX_UNLOCK(m)   __gthread_mutex_unlock(m)
#	define COND_T            __gthread_cond_t
#	define COND_INIT         __GTHREAD_COND_INIT
#	define COND_WAIT(cv,m)   __gthread_cond_wait(cv,m)
#	define COND_SIGNAL(cv)   __gthread_cond_signal(cv)
#endif

class semaphore {
private:
	MUTEX_T x_m;
	COND_T x_cv;
	std::size_t x_n;

public:
	explicit semaphore(std::size_t n_init = 0)
		: x_m(MUTEX_INIT), x_cv(COND_INIT), x_n(n_init)
	{
	}

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
			__builtin_abort();
		}
		++x_n;
		COND_SIGNAL(&x_cv);
		MUTEX_UNLOCK(&x_m);
	}
};

int main(){
	constexpr std::size_t n_threads = 4;
	constexpr std::size_t n_loops = 1000000;

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

	std::cout <<"n_ping = " <<n_ping.load(std::memory_order_relaxed) <<std::endl
	          <<"n_pong = " <<n_pong.load(std::memory_order_relaxed) <<std::endl
	          <<"delta_milliseconds = " <<std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() <<std::endl;
}

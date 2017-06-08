// This file is put into the Public Domain.

#include "../src/gthread.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

// tls
__gthread_key_t key;

void tls_destructor(void *p){
	assert(__gthread_getspecific(key) == 0);

	printf("destructing tls data %u\n", *(unsigned *)p);
	free(p);
}

// mutex
__gthread_mutex_t mutex = __GTHREAD_MUTEX_INIT;
volatile unsigned long counter = 0;

#define INCREMENT_PER_THREAD   1000000ul
#define THREAD_COUNT           4ul

void *test_thread_proc(void *param){
	unsigned *p = malloc(sizeof(unsigned));
	assert(p);
	*p = (unsigned)(uintptr_t)param;
	int err = __gthread_setspecific(key, p);
	assert(err == 0);
	printf("constructed tls data %u\n", *(unsigned *)p);

	err = __gthread_setspecific(key, p);
	assert(err == 0);
	printf("set new tls data %u\n", *(unsigned *)p);

	for(unsigned long i = 0; i < INCREMENT_PER_THREAD; ++i){
		__gthread_mutex_lock(&mutex);
		unsigned c = counter;
		++c;
		counter = c;
		__gthread_mutex_unlock(&mutex);
	}

	return (void *)(uintptr_t)*(unsigned *)p;
}

int main(){
	int err = __gthread_key_create(&key, &tls_destructor);
	assert(err == 0);
	printf("key = %p\n", (void *)key);

	__gthread_t threads[THREAD_COUNT];
	for(unsigned i = 0; i < THREAD_COUNT; ++i){
		err = __gthread_create(&threads[i], &test_thread_proc, (void *)(intptr_t)i);
		assert(err == 0);
	}
	for(unsigned i = 0; i < THREAD_COUNT; ++i){
		printf("waiting for thread %u\n", i);
		void *exit_code;
		err = __gthread_join(threads[i], &exit_code);
		assert(err == 0);
		printf("exit code was %u\n", (unsigned)(uintptr_t)exit_code);
	}
	printf("counter = %lu\n", counter);

	__gthread_key_delete(key);

	assert(counter == INCREMENT_PER_THREAD * THREAD_COUNT);
}

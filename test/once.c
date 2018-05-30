// This file is put into the Public Domain.

#include "../src/gthread.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <windows.h>

// once
__gthread_once_t once = __GTHREAD_ONCE_INIT;
unsigned long counter = 0;

void once_proc(void){
	printf("- performing once initialization: counter = %lu\n", counter);
	int old = counter;
	Sleep(1000);
	counter = old + 1;
	printf("- done once initialization: counter = %lu\n", counter);
}

#define THREAD_COUNT           16ul

void * test_thread_proc(void *param){
	printf("thread %u running\n", (unsigned)(intptr_t)param);

	Sleep(500);

	int err = __gthread_once(&once, &once_proc);
	assert(err == 0);

	printf("thread %u exiting\n", (unsigned)(intptr_t)param);
	return 0;
}

int main(){
	int err;
	__gthread_t threads[THREAD_COUNT];
	for(unsigned i = 0; i < THREAD_COUNT; ++i){
		err = __gthread_create(&threads[i], &test_thread_proc, (void *)(intptr_t)i);
		assert(err == 0);
	}
	for(unsigned i = 0; i < THREAD_COUNT; ++i){
		printf("waiting for thread %u\n", i);
		err = __gthread_join(threads[i], 0);
		assert(err == 0);
	}
	printf("counter = %lu\n", counter);

	assert(counter == 1);
}

// This file is put into the Public Domain.

#include "../src/env/c11thread.h"

#include <stdio.h>

int thread_proc(void *param){
	const thrd_t self = thrd_current();
	printf("+ thread running: tid = %u\n", (unsigned)self);

	const thrd_t init = (thrd_t)(intptr_t)param;
	printf("+ joining initial thread: init_tid = %u\n", (unsigned)init);
	int exit_code;
	int err = thrd_join(init, &exit_code);
	printf("+ joined initial thread: err = %d, exit_code = %d\n", err, exit_code);
	return 43;
}

int main(){
	const thrd_t self = thrd_current();
	printf("- initial thread running: tid = %u\n", (unsigned)self);

	thrd_t child;
	int err = thrd_create(&child, thread_proc, (void *)(intptr_t)self);
	printf("- created child thread: err = %d, child_tid  = %u\n", err, (unsigned)child);
	thrd_exit(42);
}

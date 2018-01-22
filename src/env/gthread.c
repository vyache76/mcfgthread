// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2018, LH_Mouse. All wrongs reserved.

#define __MCFCRT_GTHREAD_INLINE_OR_EXTERN     extern inline
#include "gthread.h"

void __MCFCRT_gthread_tls_destructor(intptr_t context, void *storage){
	void (*const destructor)(void *) = (void (*)(void *))context;

	void *const value = *(void **)storage;
	if(!value){
		return;
	}
	*(void **)storage = _MCFCRT_NULLPTR;
	(*destructor)(value);
}

intptr_t __MCFCRT_gthread_unlock_callback_mutex(intptr_t context){
	__gthread_mutex_t *const mutex = (__gthread_mutex_t *)context;

	__gthread_mutex_unlock(mutex);
	return 1;
}
void __MCFCRT_gthread_relock_callback_mutex(intptr_t context, intptr_t unlocked){
	__gthread_mutex_t *const mutex = (__gthread_mutex_t *)context;

	_MCFCRT_ASSERT((size_t)unlocked == 1);
	__gthread_mutex_lock(mutex);
}

intptr_t __MCFCRT_gthread_unlock_callback_recursive_mutex(intptr_t context){
	__gthread_recursive_mutex_t *const recur_mutex = (__gthread_recursive_mutex_t *)context;
	_MCFCRT_ASSERT(_MCFCRT_GetCurrentThreadId() == __atomic_load_n(&(recur_mutex->__owner), __ATOMIC_RELAXED));

	const size_t old_count = recur_mutex->__count;
	recur_mutex->__count = 0;
	__atomic_store_n(&(recur_mutex->__owner), 0, __ATOMIC_RELAXED);

	__gthread_mutex_unlock(&(recur_mutex->__mutex));
	return (intptr_t)old_count;
}
void __MCFCRT_gthread_relock_callback_recursive_mutex(intptr_t context, intptr_t unlocked){
	__gthread_recursive_mutex_t *const recur_mutex = (__gthread_recursive_mutex_t *)context;

	_MCFCRT_ASSERT((size_t)unlocked >= 1);
	__gthread_mutex_lock(&(recur_mutex->__mutex));

	const uintptr_t self = _MCFCRT_GetCurrentThreadId();
	__atomic_store_n(&(recur_mutex->__owner), self, __ATOMIC_RELAXED);
	recur_mutex->__count = (size_t)unlocked;
}

void __MCFCRT_gthread_mopthread_wrapper(void *params){
	__MCFCRT_gthread_control_t *const control = params;

	void *const exit_code = (*(control->__proc))(control->__param);
	control->__exit_code = exit_code;
}

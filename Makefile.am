ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = no-exeext foreign subdir-objects
AM_CPPFLAGS = -std=c11 -Wall -Wextra -pedantic -pedantic-errors -Wstrict-prototypes -Wconversion -Wsign-conversion -Wsuggest-attribute=noreturn -Wundef -Wshadow	\
	-pipe -mfpmath=both -march=nocona -mno-stack-arg-probe -mno-accumulate-outgoing-args -mpush-args -masm=intel

## I think you GNU people should just STFU and stop confusing the linker.
EXEEXT =

pkginclude_HEADERS =	\
	src/gthread.h	\
	src/c11thread.h

pkginclude_envdir = $(pkgincludedir)/env
pkginclude_env_HEADERS =	\
	src/env/avl_tree.h	\
	src/env/bail.h	\
	src/env/clocks.h	\
	src/env/condition_variable.h	\
	src/env/gthread.h	\
	src/env/c11thread.h	\
	src/env/heap.h	\
	src/env/mcfwin.h	\
	src/env/mutex.h	\
	src/env/once_flag.h	\
	src/env/thread.h	\
	src/env/thread_env.h	\
	src/env/_crtdef.h	\
	src/env/_gthread_inl.h	\
	src/env/_c11thread_inl.h

pkginclude_extdir = $(pkgincludedir)/ext
pkginclude_ext_HEADERS =	\
	src/ext/assert.h	\
	src/ext/expect.h	\
	src/ext/itow.h	\
	src/ext/wcpcpy.h

bin_PROGRAMS =	\
	bin/mcfgthread.dll

bin_mcfgthread_dll_SOURCES =	\
	src/env/avl_tree.c	\
	src/env/bail.c	\
	src/env/clocks.c	\
	src/env/condition_variable.c	\
	src/env/gthread.c	\
	src/env/c11thread.c	\
	src/env/heap.c	\
	src/env/mutex.c	\
	src/env/once_flag.c	\
	src/env/thread.c	\
	src/env/thread_env.c	\
	src/env/_eh_top.c	\
	src/ext/assert.c	\
	src/ext/itow.c	\
	src/ext/wcpcpy.c	\
	src/dll_startup.c

bin_mcfgthread_dll_LDFLAGS =	\
	-shared -static -nostdlib	\
	-Wl,-e__MCFCRT_DllStartup,-subsystem,windows,--disable-runtime-pseudo-reloc,--disable-auto-import	\
	-Wl,--disable-stdcall-fixup,--enable-auto-image-base,--export-all-symbols,--out-implib,libmcfgthread.dll.a

bin_mcfgthread_dll_LDADD =	\
	-lkernel32 -lntdll

lib_LIBRARIES =	\
	lib/libmcfgthread.dll.a	\
	lib/libmcfgthread.a

lib_libmcfgthread_dll_a_SOURCES =

lib/libmcfgthread.dll.a: bin/mcfgthread.dll
	mkdir -p "$$(dirname '$@')"
	cp -p libmcfgthread.dll.a '$@'

lib_libmcfgthread_a_SOURCES =

lib/libmcfgthread.a: bin/mcfgthread.dll
	mkdir -p "$$(dirname '$@')"
	## Due to Windows limitation we can't link this library statically.
	cp -p libmcfgthread.dll.a '$@'

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -std=c11 -Wall -Wextra -pedantic -pedantic-errors -Wconversion -Wsign-conversion -Wsuggest-attribute=noreturn -Wundef -Wshadow	\
	-pipe -mfpmath=both -march=nocona -mno-stack-arg-probe -mno-accumulate-outgoing-args -mpush-args -masm=intel

pkginclude_HEADERS =	\
	src/gthread.h

pkginclude_envdir = $(pkgincludedir)/env
pkginclude_env_HEADERS =	\
	src/env/avl_tree.h	\
	src/env/bail.h	\
	src/env/clocks.h	\
	src/env/condition_variable.h	\
	src/env/gthread.h	\
	src/env/mcfwin.h	\
	src/env/mutex.h	\
	src/env/once_flag.h	\
	src/env/thread.h	\
	src/env/thread_env.h	\
	src/env/_crtdef.h	\
	src/env/_nt_timeout.h

pkginclude_extdir = $(pkgincludedir)/ext
pkginclude_ext_HEADERS =	\
	src/ext/assert.h	\
	src/ext/expect.h	\
	src/ext/itow.h	\
	src/ext/wcpcpy.h

lib_LTLIBRARIES =	\
	lib/libmcfgthread.la

lib_libmcfgthread_la_SOURCES =	\
	src/env/avl_tree.h	\
	src/env/bail.h	\
	src/env/mcfwin.h	\
	src/env/thread_env.h	\
	src/env/_crtdef.h	\
	src/env/_nt_timeout.h	\
	src/ext/assert.h	\
	src/ext/expect.h	\
	src/ext/itow.h	\
	src/ext/wcpcpy.h	\
	src/dll_main.c	\
	src/env/avl_tree.c	\
	src/env/bail.c	\
	src/env/clocks.c	\
	src/env/condition_variable.c	\
	src/env/gthread.c	\
	src/env/mutex.c	\
	src/env/once_flag.c	\
	src/env/thread.c	\
	src/env/thread_env.c	\
	src/ext/assert.c	\
	src/ext/itow.c	\
	src/ext/wcpcpy.c

EXTRA_DIST =	\
	build_it.sh	\
	test/build_test.sh	\
	test/test.c

lib_libmcfgthread_la_LDFLAGS =	\
	-version-info 1 -shared -no-undefined -nodefaultlibs	\
	-Wl,-lgcc,-lgcc_eh,-lmsvcrt,-lkernel32,-lntdll,-subsystem,windows,--disable-runtime-pseudo-reloc,--disable-auto-import

install-exec-hook:
## Due to Windows limitation we can't link this library statically.
	(cd $(libdir) &&	\
		ln -sf libmcfgthread.dll.a libmcfgthread.a)

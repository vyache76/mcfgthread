#!/bin/sh -e

_prefix="$(pwd)/debug/mingw32"
_builddir="$(pwd)/tmp/build_i686_debug"
_build=i686-w64-mingw32

mkdir -p "${_prefix}"
mkdir -p "${_builddir}"

echo Building shared library...
mkdir -p m4
autoreconf -i
(cd "${_builddir}" &&
  (test -f Makefile ||  \
    CPPFLAGS=''  \
    CFLAGS='-O0 -g'  \
    LDFLAGS=''  \
    ../configure --_build="${_build}" --host="${_build}" --_prefix="${_prefix}") &&
  make -j4)

echo Testing...
cp -p "${_builddir}/libmcfgthread.dll.a" "${_builddir}/libmcfgthread-new.dll.a"
("${_build}"-gcc -std=c11 -Wall -Wextra -pedantic test/test.c -Isrc/env -L"${_builddir}" -lmcfgthread-new -o test.exe &&
  PATH="${_builddir}:$PATH" ./test.exe)

(cd "${_builddir}" &&
  make install)

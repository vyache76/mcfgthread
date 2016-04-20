#!/bin/sh

set -e

prefix="$(pwd)/release/mingw32"
mkdir -p "$prefix"

build="$(pwd)/build-i686"
mkdir -p "$build"

echo Building shared library...
mkdir -p m4
autoreconf -i
(cd "$build" &&
  CFLAGS='-O3' CPPFLAGS='-DNDEBUG' LDFLAGS='-Wl,-s' ../configure --disable-static --enable-shared --build=i686-w64-mingw32 --host=i686-w64-mingw32 --prefix="$prefix" &&
  make -j4)

echo Testing...
(gcc -std=c11 -Wall -Wextra -Wpedantic -Werror test/test.c -Isrc/env -L"$build/lib/.libs" -lmcfgthread -o test.exe &&
  PATH=$PATH:"$build/lib/.libs" ./test.exe)

(cd "$build" &&
  make install)
# Due to Windows limitation we can't link this library as a static one.
(cd "$prefix/lib" &&
  ln -sf libmcfgthread.dll.a libmcfgthread.a)

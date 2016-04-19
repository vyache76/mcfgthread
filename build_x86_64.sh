#!/bin/sh

set -e

prefix="$(pwd)/release/mingw64"

echo Building shared library...
mkdir -p m4
autoreconf -i
CFLAGS='-O3' CPPFLAGS='-DNDEBUG' LDFLAGS='-Wl,-s' ./configure --disable-static --enable-shared --build=x86_64-w64-mingw32 --host=x86_64-w64-mingw32 --prefix="$prefix"
make clean
make -j4

echo Testing...
(cd test && ./build_test.sh)

make install
# Due to Windows limitation we can't link this library as a static one.
(cd "$prefix/lib" && ln -sf libmcfgthread.dll.a libmcfgthread.a)

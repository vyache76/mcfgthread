#!/bin/sh

set -e

echo Building shared library...
mkdir -p m4
autoreconf -i
CFLAGS='-O3' LDFLAGS='-Wl,-s' ./configure --disable-static --enable-shared --build=x86_64-w64-mingw32 --host=x86_64-w64-mingw32 --prefix="$(pwd)/release/mingw64"
make clean
make -j4

echo Testing...
(cd test && ./build_test.sh)

make install

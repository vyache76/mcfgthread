#!/bin/sh

set -e

echo Building shared library...
mkdir -p m4
autoreconf -i
CFLAGS='-O3' LDFLAGS='-Wl,-s' ./configure --disable-static --enable-shared --build=i686-w64-mingw32 --host=i686-w64-mingw32 --prefix="$(pwd)/release/mingw32"
make clean
make -j4

echo Testing...
(cd test && ./build_test.sh)

make install

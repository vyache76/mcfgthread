#!/bin/sh

set -e

echo Building shared library...
mkdir -p m4
autoreconf -if
CFLAGS='-O3' ./configure --disable-static --enable-shared
make -j4

echo Testing...
(cd test && ./build_test.sh)

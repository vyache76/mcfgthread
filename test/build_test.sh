#!/bin/sh

set -e

gcc -std=c11 -Wall -Wextra -Wpedantic -Werror test.c -I../src/env -L../lib/.libs -lmcfgthread -otest.exe
PATH=$PATH:"$(pwd)/../lib/.libs" ./test.exe

#!/bin/bash

set -e

function check_one(){
	_file="$1"
	_flags="-Wall -Wextra -Werror"
	_cmd="i686-w64-mingw32-gcc -std=c99 ${_flags} -x c -c -o /dev/null"
	echo "Checking \`#include\` directives:  ${_cmd}  \"${_file}\""
	${_cmd}  "${_file}"
	_cmd="i686-w64-mingw32-gcc -std=c++11 ${_flags} -x c++ -c -o /dev/null"
	echo "Checking \`#include\` directives:  ${_cmd}  \"${_file}\""
	${_cmd}  "${_file}"
}

export -f check_one
find -L "src/" -name "*.h" -print0 | xargs -0 -I {} bash -c 'check_one "$@"' "$0" {}

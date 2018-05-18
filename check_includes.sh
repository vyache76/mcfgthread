#!/bin/bash

set -e

function check_one(){
	_file="$1"
	_flags="-Wall -Wextra -Werror"
	_cc=""
	i686-w64-mingw32-gcc -v &>/dev/null && _cc="i686-w64-mingw32-gcc"
	x86_64-w64-mingw32-gcc -v &>/dev/null && _cc="x86_64-w64-mingw32-gcc"
	if test -z "${_cc}"; then
		echo "No mingw-w64 compiler found." >&2
		exit 2
	fi
	_cmd="${_cc} -std=c99 ${_flags} -x c -c -o /dev/null"
	echo "Checking \`#include\` directives:  ${_cmd}  \"${_file}\""
	${_cmd}  "${_file}"
	_cmd="${_cc} -std=c++11 ${_flags} -x c++ -c -o /dev/null"
	echo "Checking \`#include\` directives:  ${_cmd}  \"${_file}\""
	${_cmd}  "${_file}"
}

export -f check_one
find -L "src/" -name "*.h" -print0 | xargs -0 -I {} bash -c 'check_one "$@"' "$0" {}

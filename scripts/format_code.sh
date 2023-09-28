#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SRC_ROOT="$DIR/.."
echo $SRC_ROOT

SRC_LIST="$(find "$SRC_ROOT" -not \( -path "$SRC_ROOT/toolchain/*" -prune \) \
							 -not \( -path "$SRC_ROOT/build/*" -prune \) \
			| grep -E ".*(\.c|\.h)$")"

echo -e "Files found to format = \n\"\"\"\n$SRC_LIST\n\"\"\""

clang-format --verbose -i --style=file $SRC_LIST
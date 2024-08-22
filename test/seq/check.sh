#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

current_dir=$(pwd)
seq=$current_dir/src/seq

check_value() {
	printf "expected:\n%s\ngot:\n%s\n" "$1" "$2"
	if [ "$1" = "$2" ]; then
		echo "check passed"
	else
		echo "check failed"
		exit 1
	fi
}

echo "Checking ranges for seq"
check_value "$(printf "1\n2\n3\n")" "$($seq 3)"
check_value "$(printf "3\n4\n5\n")" "$($seq 3 5)"
check_value "$(printf "2\n2.5\n3\n3.5\n4\n4.5\n5\n")" "$($seq 2 0.5 5)"

echo "Checking seq -w"
check_value "$(printf "001\n002\n003\n004\n")" "$($seq -w 100 | head -n 4)"

echo "Checking seq -s"
check_value "1:2:3:4" "$($seq -s ":" 4)"

echo "Checking seq pasring"
check_value "$(printf "7\n8\n9\n10\n")" "$($seq 7 1e1)"
check_value "$(printf "11\n11.5\n12\n")" "$($seq 11 0.5 +1.2e1)"

exit 0

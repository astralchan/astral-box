#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

current_dir=$(pwd)
test_dir=$(dirname "$0")

cat=$current_dir/src/cat

clean_up() {
	cd "$test_dir" || exit 1
	rm -f file*.txt
	cd "$current_dir" || exit 1
}

check_value() {
	printf "expected:\n%s\ngot:\n%s\n" "$1" "$2"
	if [ "$1" = "$2" ]; then
		echo "check passed"
	else
		echo "check failed"
		exit 1
	fi
}

trap clean_up EXIT

clean_up

echo "Hello, world!" > "$test_dir"/file1.txt
touch "$test_dir"/file2.txt
printf "Goodbye,\nworld!\n" > "$test_dir"/file3.txt

echo "Checking cat file"
check_value "Hello, world!" "$($cat "$test_dir"/file1.txt)"
check_value "" "$($cat "$test_dir"/file2.txt)"
check_value "$(printf "Goodbye,\nworld!\n")" "$($cat "$test_dir"/file3.txt)"

echo "Checking cat pipes"
check_value "Hello, world!" "$(echo "Hello, world!" | $cat)"
check_value "" "$(echo "" | $cat)"

clean_up

exit 0

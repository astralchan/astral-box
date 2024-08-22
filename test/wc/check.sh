#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

current_dir=$(pwd)
test_dir=$(dirname "$0")

wc=$current_dir/src/wc

clean_up() {
	cd "$test_dir" || exit 1
	rm -f file1.txt file2.txt file3.txt
	cd "$current_dir" || exit 1
}

trap clean_up EXIT

clean_up

# Compare $1 and $2
check_value() {
	printf "expected: %s, got: %s\n" "$1" "$2"
	if [ "$1" = "$2" ]; then
		echo "check passed"
	else
		echo "check failed"
		exit 1
	fi
}

cd "$test_dir" || exit 1

# Make test files
echo "Hello, world!" > file1.txt
touch file2.txt
echo "こんにちは、世界！" > file3.txt

cd "$current_dir" || exit 1

echo "Checking wc -c"
check_value "14" "$($wc -c "$test_dir"/file1.txt | awk '{print $1}')"
check_value "0" "$($wc -c "$test_dir"/file2.txt | awk '{print $1}')"
check_value "28" "$($wc -c "$test_dir"/file3.txt | awk '{print $1}')"

echo "Checking wc -l"
check_value "1" "$($wc -l "$test_dir"/file1.txt | awk '{print $1}')"
check_value "0" "$($wc -l "$test_dir"/file2.txt | awk '{print $1}')"
check_value "1" "$($wc -l "$test_dir"/file3.txt | awk '{print $1}')"

echo "Checking wc -w"
check_value "2" "$($wc -w "$test_dir"/file1.txt | awk '{print $1}')"
check_value "0" "$($wc -w "$test_dir"/file2.txt | awk '{print $1}')"
check_value "1" "$($wc -w "$test_dir"/file3.txt | awk '{print $1}')"

echo "Checking wc -m"
check_value "14" "$($wc -m "$test_dir"/file1.txt | awk '{print $1}')"
check_value "0" "$($wc -m "$test_dir"/file2.txt | awk '{print $1}')"
check_value "10" "$($wc -m "$test_dir"/file3.txt | awk '{print $1}')"

echo "Checking wc multiple files"
check_value "2 3 42" "$($wc "$test_dir"/file1.txt "$test_dir"/file2.txt "$test_dir"/file3.txt | tail -n 1 | awk '{print $1" "$2" "$3}')"
check_value "2 3 24" "$($wc -lwm "$test_dir"/file1.txt "$test_dir"/file2.txt "$test_dir"/file3.txt | tail -n 1 | awk '{print $1" "$2" "$3}')"

echo "Checking pipes for wc"
check_value "1 2 14" "$(cat "$test_dir"/file1.txt | $wc | awk '{print $1" "$2" "$3}')"
check_value "0 0 0" "$(cat "$test_dir"/file2.txt | $wc | awk '{print $1" "$2" "$3}')"
check_value "1 1 28" "$(cat "$test_dir"/file3.txt | $wc | awk '{print $1" "$2" "$3}')"

clean_up
exit 0

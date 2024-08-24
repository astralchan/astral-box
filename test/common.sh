#!/bin/sh

check_value() {
	printf "Expected:\n%s\nReceived:\n%s\n" "$1" "$2"
	if [ "$1" = "$2" ]; then
		echo "Check passed"
	else
		echo "Check failed"
		exit 1
	fi
}

eval "current_dir=$(pwd)"
eval "test_dir=${current_dir}/$(dirname "$0")"
eval "command=${current_dir}/src/${test_dir##*/}"

clean_up() {
	cd "$test_dir" || exit 1
	rm -rf file*
}

trap clean_up EXIT

cd "$test_dir" || exit 1

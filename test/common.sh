#!/bin/sh

check_value() {
	printf "expected:\n%s\ngot:\n%s\n" "$1" "$2"
	if [ "$1" = "$2" ]; then
		echo "check passed"
	else
		echo "check failed"
		exit 1
	fi
}

eval "current_dir=$(pwd)"
eval "test_dir=${current_dir}/$(dirname "$0")"
eval "command=$(printf "%s/src/%s" "$current_dir" "${test_dir##*/}")"

clean_up() {
	cd "$test_dir" || exit 1
	rm -rf file*
}

trap clean_up EXIT

cd "$test_dir" || exit 1

#!/bin/sh

check_value() {
	printf "Expected:\n%s\n" "$1"
	printf "Received:\n%s\n" "$2"
	if [ "$1" = "$2" ]; then
		echo "Check passed"
	else
		echo "Check failed"
		exit 1
	fi
}

eval "base_dir=\$(pwd)"
eval "cmdname=\$(dirname \"\$0\" | sed \"s/.*\///\")"
eval "cmd=\${base_dir}/src/\${cmdname}"
eval "test_dir=\${base_dir}/test/${cmdname}"

clean_up() {
	cd "$test_dir" || exit 1
	for file in *; do
		if [ "$(basename "$file")" != "check.sh" ]; then
			rm "$file"
		fi
	done
}

trap clean_up EXIT

cd "$test_dir" || exit 1

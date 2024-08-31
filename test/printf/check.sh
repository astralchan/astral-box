#!/bin/sh

. test/common.sh

echo "Checking $cmdname strings..."
check_value "" "$($cmd "")"
check_value "Hello, world!" "$($cmd "Hello, world!")"

echo "Checking $cmdname formats..."
check_value "foo 42" "$($cmd "%s %d" "foo" "42")"

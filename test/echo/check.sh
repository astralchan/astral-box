#!/bin/sh

. test/common.sh

echo "Checking $cmdname with no args..."
check_value "" "$($cmd)"
echo "Checking $cmdname with string..."
check_value "Hello, world!" "$($cmd "Hello, world!")"

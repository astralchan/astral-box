#!/bin/sh

. test/common.sh

echo "Checking $cmdname return..."
if $cmd; then
	echo "Check passed"
else
	echo "Check failed"
	exit 1
fi

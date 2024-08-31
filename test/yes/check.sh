#!/bin/sh

. test/common.sh

check_value "y" "$($cmd | { read -r var; echo $var; })"
check_value "foo bar" "$($cmd foo bar | { read -r var; echo $var; })"

#!/bin/sh

. test/common.sh

# Generate test files
echo "Generating test directories..."
mkdir real
ln -sf real link

cd link

echo "Checking $cmdname -L..."
check_value "${test_dir}/link" "$($cmd -L)"
echo "Checking $cmdname -P..."
check_value "${test_dir}/real" "$($cmd -P)"
echo "Checking $cmdname -L and -P..."
check_value "${test_dir}/real" "$($cmd -LP)"
echo "Checking $cmdname no option..."
check_value "${test_dir}/link" "$($cmd)"

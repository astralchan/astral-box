#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

if ! ./src/false; then
    echo "false test passed"
else
    echo "false test failed"
    exit 1
fi

exit 0

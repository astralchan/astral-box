#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

if ./src/true; then
    echo "true test passed"
else
    echo "true test failed"
    exit 1
fi

exit 0

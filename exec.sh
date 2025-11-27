#!/bin/bash

if ! make -j 4; then
    exit 1
fi

if [ -f "./bin/main" ]; then
    ./bin/main
else
    echo "File ./bin/main does not exist"
fi
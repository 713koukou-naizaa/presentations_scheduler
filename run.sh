#!/bin/bash

if [ -f "./bin/main" ]; then
    ./bin/main "$@"
else
    echo "File ./bin/main does not exist"
fi
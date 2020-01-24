#!/bin/bash

# Build the compiler
#
# You MUST replace the following commands with the commands for building your compiler

if [ -f "src/minic" ]; then
    make clean -C ./src
fi
make minic -C ./src

#!/bin/bash

# Variables
SRC_DIR="src"
OUTPUT_FILE="jvlisp.exe"

gcc $SRC_DIR/main.c -o $OUTPUT_FILE

# if [ $? -eq 0 ]; then
#   echo "Compilation successful!"
# else
#   echo "Compilation error!"
# fi

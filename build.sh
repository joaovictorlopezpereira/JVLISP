#!/bin/bash

# Variables
SRC_DIR="src"
INCLUDE_DIR="include"
OUTPUT_FILE="jvlisp.exe"

gcc -I$INCLUDE_DIR -o $OUTPUT_FILE $SRC_DIR/*.c

# if [ $? -eq 0 ]; then
#   echo "Compilation successful!"
# else
#   echo "Compilation error!"
# fi

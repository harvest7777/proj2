#!/bin/bash

# Exit on first error
set -e

# Compile main.c into dining.exe
gcc main.c -o dining -pthread

# If successful, print a message
echo "Compiled successfully -> ./dining"


#!/bin/bash

# create build directory if it doesn't exist
if [ ! -d "build" ]; then
  mkdir build
fi

# compile all C files in src directory
for file in src/*.c; do
  gcc -c $file -o build/$(basename ${file%.c}.o)
done

# link all object files together to create executable
gcc build/*.o -o serveur
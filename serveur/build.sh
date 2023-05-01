#!/bin/bash

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
  mkdir build
fi

# Compile all C files in src directory and save object files in build directory
for file in src/*.c; do
  gcc -c $file -o build/$(basename ${file%.c}.o)
done

# Link all object files together to create executable named "serveur"
# Use the "-pthread" flag to link with the pthread library for threads and mutexes
gcc -pthread build/*.o -o serveur

# Execute the newly created "serveur" executable
./serveur

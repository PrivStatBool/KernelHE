#!/bin/bash

# Change to the script's directory
cd "$(dirname "$0")"

# Compile the program
gcc -o testPlainKmeans_new_new testPlainKmeans_new_new.c -lm

# binary is executable
chmod 755 testPlainKmeans_new_new

# Run the program
./testPlainKmeans_new_new


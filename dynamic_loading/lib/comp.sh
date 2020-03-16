#!/bin/bash

for name in $(ls *.o 2> /dev/null); do
    echo "Deleting ${name}"
    rm "${name}"
done

for name in $(ls libstateful_writer.* 2> /dev/null); do
    echo "Deleting ${name}"
    rm "${name}"  
done

# Let's add complete symbol table and set the name of the library for compatibility
EXTRA_LINKER_OPT="-rdynamic -Wl,-soname,libstateful_writer.so"

# Compiles the source code and generates the object file stateful_writer.o 
g++ -std=c++17 -fPIC -Wall -c stateful_writer.cpp

# Generates the static library libstateful_writer.a including the object stateful_writer.o
ar r libstateful_writer.a stateful_writer.o

# Generates a shared library libstateful_writer.so
g++ -std=c++17 -Wall -shared ${EXTRA_LINKER_OPT} -o libstateful_writer.so stateful_writer.o

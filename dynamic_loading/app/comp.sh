#!/bin/bash

for name in $(ls *.o); do
    echo "Deleting ${name}"
    rm "${name}"
done

for name in app app_static app_dyn; do
    echo "Deleting ${name}"
    [[ -f "${name}" ]] && rm "${name}"
done

# Compiles the source code and generates the object file main.o
# ../lib points to the location of the file stateful_writer.h
g++ -Wall -c main.cpp -I../lib

# Generates the executable binary app_static including the static library libstateful_writer.a
# Adding this .a file is the same as adding all .o files contained in this library
g++ -o app_static main.o ../lib/libstateful_writer.a

# Generates a executable binary app using the shared library libstateful_writer.so
g++ -o app main.o -L../lib -lstateful_writer

# Compiles the source code and generates the object file main_dyn.o
# ../lib points to the location of the file stateful_writer_imports.h
g++ -I../lib -Wall -c main_dyn.cpp

# Generates a executable binary app without linking any library
g++ -Wall main_dyn.o -ldl -o app_dyn

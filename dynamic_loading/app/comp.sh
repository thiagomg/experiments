#!/bin/bash

for name in $(ls *.o); do
    echo "Deleting ${name}"
    rm "${name}"
done

for name in app app_static app_dyn; do
    echo "Deleting ${name}"
    rm "${name}"
done

g++ -Wall -c main.cpp -I/home/thiago/src/teste/lib
g++ -o app main.o -L/home/thiago/src/teste/lib -lstateful_writer
g++ -o app_static main.o ../lib/libstateful_writer.a

g++ -I/home/thiago/src/teste/lib -Wall -c main_dyn.cpp
g++ -rdynamic -Wall main_dyn.o -ldl -o app_dyn

#include <iostream>

#include <stateful_writer.h>

int main(int argc, char **argv) {
    stateful_writer w;
    w.write("Thiago");
    w.write("Massari");
    w.write("Guedes");

    std::cout << "how: " << w.increment(1) << std::endl;
    std::cout << "how: " << w.increment(2) << std::endl;
    std::cout << "how: " << w.increment(3) << std::endl;
}
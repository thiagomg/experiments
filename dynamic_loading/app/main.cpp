#include <iostream>

#include <stateful_writer.h>

int main(int argc, char **argv) {
    stateful_writer w;
    w.write("We're all stories");
    w.write("in the end");

    std::cout << "how much: " << w.increment(10) << std::endl;
    std::cout << "how much: " << w.increment(10) << std::endl;
    std::cout << "how much: " << w.increment(10) << std::endl;
}

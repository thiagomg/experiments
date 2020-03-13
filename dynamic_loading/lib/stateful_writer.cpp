#include "stateful_writer.h"

#include <iostream>

int stateful_writer::increment(int how_much) noexcept {
    _counter++;
    return how_much + _counter;
}

void stateful_writer::write(const std::string &what) noexcept {
    ++_counter;
    std::cout << _counter << " " << what << std::endl;
}

extern "C" {

void *new_writer() {
    stateful_writer *w = new stateful_writer();
    return (void *)w;
}

void delete_writer(void *writer_handle) {
    stateful_writer *w = (stateful_writer *)writer_handle;
    delete w;
}

int stateful_increment(void *writer_handle, int val) {
    stateful_writer *w = (stateful_writer *)writer_handle;
    return w->increment(val);
}

void stateful_write(void *writer_handle, const char *c) {
    stateful_writer *w = (stateful_writer *)writer_handle;
    return w->write(c);
}

}
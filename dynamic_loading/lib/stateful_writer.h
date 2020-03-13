#pragma once

#include <string>

struct stateful_writer {
    int _counter {};
    int increment(int how_much) noexcept;
    void write(const std::string &what) noexcept;
};
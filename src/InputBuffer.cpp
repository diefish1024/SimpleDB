#include "InputBuffer.hpp"
#include <cstdlib>
#include <iostream>

void InputBuffer::readInput() {
    std::getline(std::cin, buffer);
    if(std::cin.fail()) {
        std::cerr << "Error reading input" << std::endl;
        std::exit(1);
    }
}

const std::string& InputBuffer::getBuffer() const {
    return buffer;
}
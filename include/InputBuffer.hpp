#ifndef INPUTBUFFER_HPP
#define INPUTBUFFER_HPP

#include <string>

class InputBuffer {
private:
    std::string buffer;

public:
    InputBuffer() = default;
    ~InputBuffer() = default;

    void readInput();
    
    const std::string& getBuffer() const;
};

#endif // INPUTBUFFER_HPP
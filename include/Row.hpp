#ifndef ROW_HPP
#define ROW_HPP

#include <iostream>
#include <string>
#include <cstdint>

const uint32_t COLUMN_USERNAME_MAX_LENGTH = 32;
const uint32_t COLUMN_EMAIL_MAX_LENGTH = 255;

struct Row {
    int id;
    std::string username;
    std::string email;

    void print() const {
        std::cout << id << " " << username << " " << email << std::endl;
    }
};


#endif // ROW_HPP
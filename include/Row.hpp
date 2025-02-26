#ifndef ROW_HPP
#define ROW_HPP

#include "Constants.hpp"
#include <iostream>
#include <string>
#include <cstdint>

struct Row {
    int id;
    char username[Constants::COLUMN_USERNAME_MAX_LENGTH+ 1];
    char email[Constants::COLUMN_EMAIL_MAX_LENGTH + 1];
};

void printRow(const Row& row);

#endif // ROW_HPP
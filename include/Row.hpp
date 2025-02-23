#ifndef ROW_HPP
#define ROW_HPP

#include <iostream>
#include <string>
#include <cstdint>

const uint32_t COLUMN_USERNAME_MAX_LENGTH = 32;
const uint32_t COLUMN_EMAIL_MAX_LENGTH = 255;

struct Row {
    int id;
    char username[COLUMN_USERNAME_MAX_LENGTH + 1];
    char email[COLUMN_EMAIL_MAX_LENGTH + 1];
};

const uint32_t ROW_SIZE = sizeof(Row);

void printRow(const Row& row);


#endif // ROW_HPP
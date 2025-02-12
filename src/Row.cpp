#include "Row.hpp"
#include <iostream>

void printRow(const Row& row) {
    std::cout << row.id << " " << row.username << " " << row.email << std::endl;
}
#include "Row.hpp"
#include <iostream>

void Row_print(const Row& row) {
    std::cout << row.id << " " << row.username << " " << row.email << std::endl;
}
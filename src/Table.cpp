#include "Table.hpp"
#include <iostream>

Table::Table() {
    rows.reserve(MAX_ROWS);
}

Table::~Table() = default;

bool Table::insertRow(const Row& row) {
    if (rows.size() >= MAX_ROWS) {
        std::cerr << "Error: Table is full" << std::endl;
        return false;
    }
    if (row.username.size() > COLUMN_USERNAME_MAX_LENGTH) {
        std::cerr << "Error: Username is too long" << std::endl;
        return false;
    }
    if (row.email.size() > COLUMN_EMAIL_MAX_LENGTH) {
        std::cerr << "Error: Email is too long" << std::endl;
        return false;
    }

    rows.push_back(row);
    return true;
}

void Table::selectAll() const {
    for (const auto& row : rows) {
        row.print();
    }
}
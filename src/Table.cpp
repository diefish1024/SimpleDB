#include "Table.hpp"
#include <iostream>

Table::Table() {
    rows.reserve(MAX_ROWS);
}

Table::~Table() = default;

bool Table::insertRow(const Row& row) {
    if (rows.size() >= MAX_ROWS) {
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
#ifndef TABLE_HPP
#define TABLE_HPP

#include <vector>
#include <cstdint>
#include "Row.hpp"

class Table {
public:
    Table();
    ~Table();

    bool insertRow(const Row& row);
    void selectAll() const;

private:
    std::vector<Row> rows;
    static const uint32_t MAX_ROWS = 10000;
};

#endif // TABLE_HPP
#ifndef TABLE_HPP
#define TABLE_HPP

#include <vector>
#include <cstdint>
#include "Row.hpp"
#include "Pager.hpp"

const uint32_t MAX_ROWS = 10000;

class Table {
public:
    explicit Table(const std::string& filename, Pager* pager);
    ~Table();

    bool insertRow(const Row& row);
    void selectAll() const;

    void flush(); // Ensure all changes are written to disk

private:
    Pager* pager;
    uint32_t num_rows;
};

#endif // TABLE_HPP
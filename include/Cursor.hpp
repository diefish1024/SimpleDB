#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "Pager.hpp"

class Table;

class Cursor {
public:
    Table* table;
    uint32_t page_num;
    uint32_t cell_num;
    uint32_t row_num;
    bool end_of_table;

    explicit Cursor(Table* table);
    static Cursor* tableStart(Table* table);
    static Cursor* tableEnd(Table* table);

    void advance();
    char* value();
    Row& getRow();
    void insert(const Row& row);
};

#endif // CURSOR_HPP
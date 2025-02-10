#include "Table.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

Table::Table(const std::string& filename, Pager* pager) : pager(pager) {
    num_rows = pager->file_length / ROW_SIZE;
}

Table::~Table() {
    flush();
}

bool Table::insertRow(const Row& row) {
    if (num_rows >= MAX_ROWS) {
        return false;
    }

    uint32_t page_num = num_rows / ROWS_PER_PAGE;
    void* page = pager->getPage(page_num);
    uint32_t row_offset = (num_rows % ROWS_PER_PAGE) * ROW_SIZE;

    std::memcpy(static_cast<char*>(page) + row_offset, &row, ROW_SIZE);
    pager->markDirty(page_num);

    ++num_rows;
    return true;
}

void Table::selectAll() const {
    for (uint32_t i = 0; i < num_rows; ++i) {
        uint32_t page_num = i / ROWS_PER_PAGE;
        void* page = pager->getPage(page_num);
        uint32_t row_offset = i % ROWS_PER_PAGE * ROW_SIZE;
        Row* row = reinterpret_cast<Row*>(static_cast<char*>(page) + row_offset);
        row->print();
    }
}

void Table::flush() {
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; ++i) {
        pager->flush(i);
    }
}
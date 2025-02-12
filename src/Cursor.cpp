#include "Cursor.hpp"
#include "Pager.hpp"
#include "Table.hpp"
#include <cstring>

Cursor::Cursor(Table* table) 
    : table(table),
      page_num(0),
      cell_num(0),
      row_num(0),
      end_of_table(false) {
}

Cursor* Cursor::tableStart(Table* table) {
    return table->start();
}

Cursor* Cursor::tableEnd(Table* table) {
    return table->end();
}

void Cursor::advance() {
    void* page = table->pager->getPage(page_num);
    ++cell_num;
    ++row_num;
    if (cell_num >= ROWS_PER_PAGE) {
        ++page_num;
        cell_num = 0;
    }
    if (row_num >= table->meta.NUM_ROWS) {
        end_of_table = true;
    }
}

char* Cursor::value() {
    void* page = table->pager->getPage(page_num);
    return static_cast<char*>(page) + cell_num * ROW_SIZE;
}

Row& Cursor::getRow() {
    void* page = table->pager->getPage(page_num);
    return *reinterpret_cast<Row*>(static_cast<char*>(page) + cell_num * ROW_SIZE);
}

void Cursor::insert(const Row& row) {
    std::memcpy(value(), &row, ROW_SIZE);
    table->pager->markDirty(page_num);
    ++table->meta.NUM_ROWS;
    table->storeMetaData();
}
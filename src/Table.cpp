#include "Table.hpp"
#include "Cursor.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

Table::Table(const std::string& filename, Pager* pager) : pager(pager) {
    loadMetaData();
}

Table::~Table() {
    flush();
}

void Table::loadMetaData() {
    /*
     * if there multiple metadata pages, we can use the following code: 
     *
     *    char* meta_buffer = new char[MAX_META_PAGES * PAGE_SIZE];
     *    for (uint32_t i = 0; i < MAX_META_PAGES; ++i) {
     *        void* meta_page = pager->getPage(i);
     *        std::memcpy(meta_buffer + i * PAGE_SIZE, meta_page, PAGE_SIZE);
     *    }
     *    std::memcpy(&meta, meta_buffer + METADATA_OFFSET, METADATA_SIZE);
     *    delete[] meta_buffer;
     */
    void* meta_page = pager->getPage(METADATA_PAGE_NUM);
    std::memcpy(&meta, static_cast<char*>(meta_page) + METADATA_OFFSET, METADATA_SIZE);

    if (pager->file_length == (METADATA_PAGE_NUM + 1) * PAGE_SIZE) {
        meta.NUM_ROWS = 0;
        storeMetaData();
    }
}

void Table::storeMetaData() {
    /*
     * if there multiple metadata pages, we do as loadaMetaData does.
     */
    void* meta_page = pager->getPage(METADATA_PAGE_NUM);
    std::memcpy(static_cast<char*>(meta_page) + METADATA_OFFSET, &meta, METADATA_SIZE);
    pager->markDirty(METADATA_PAGE_NUM);
}

Cursor* Table::start() {
    Cursor* cursor = new Cursor(this);
    cursor->page_num = MAX_META_PAGES;
    cursor->cell_num = 0;
    cursor->end_of_table = meta.NUM_ROWS == 0;
    return cursor;
}

Cursor* Table::end() {
    Cursor* cursor = new Cursor(this);
    cursor->page_num = meta.NUM_ROWS / ROWS_PER_PAGE + MAX_META_PAGES;
    cursor->cell_num = meta.NUM_ROWS % ROWS_PER_PAGE;
    cursor->row_num = meta.NUM_ROWS;
    cursor->end_of_table = true;
    return cursor;
}

// bool Table::insertRow(const Row& row) {
//     if (meta.NUM_ROWS > MAX_ROWS) {
//         return false;
//     }

//     uint32_t page_num = meta.NUM_ROWS / ROWS_PER_PAGE + MAX_META_PAGES;
//     void* page = pager->getPage(page_num);
//     uint32_t row_offset = (meta.NUM_ROWS % ROWS_PER_PAGE) * ROW_SIZE;

//     std::memcpy(static_cast<char*>(page) + row_offset, &row, ROW_SIZE);
//     pager->markDirty(page_num);

//     ++meta.NUM_ROWS;
//     return meta.NUM_ROWS <= MAX_ROWS;
// }

// void Table::selectAll() const {
//     for (uint32_t i = 0; i < meta.NUM_ROWS; ++i) {
//         uint32_t page_num = i / ROWS_PER_PAGE + MAX_META_PAGES;
//         void* page = pager->getPage(page_num);
//         uint32_t row_offset = i % ROWS_PER_PAGE * ROW_SIZE;
//         Row* row = reinterpret_cast<Row*>(static_cast<char*>(page) + row_offset);
//         printRow(*row);
//     }
// }

void Table::flush() {
    storeMetaData();

    for (uint32_t i = 0; i < MAX_PAGES; ++i) {
        pager->flush(i);
    }
}
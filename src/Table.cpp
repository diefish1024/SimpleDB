#include "Table.hpp"
#include "Cursor.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

Table::Table(Pager* pager) : pager(pager) {
    loadMetaData();
    uint32_t root_page_num = ROOT_PAGE_NUM;
    tree = new BPlusTree(pager, root_page_num);
    BPlusNode* root = tree->getNode(root_page_num);
    if (pager->tot_pages == root_page_num + 1) {
        root->is_leaf = true;
        root->num_keys = 0;
        root->parent = 0;
        root->next = 0;
        root->prev = 0;
        root->page_num = root_page_num;
        pager->flush(root_page_num);
    }
}

Table::~Table() {
    delete tree;
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

    if (pager->tot_pages == METADATA_PAGE_NUM + 1) {
        meta.NUM_ROWS = 0;
        meta.LAST_DATA_PAGE = 2;
        meta.ROWS_IN_LAST_PAGE = 0;
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

void Table::flush() {
    storeMetaData();

    for (uint32_t i = 0; i < Constants::MAX_PAGES; ++i) {
        pager->flush(i);
    }
}

bool Table::insertRow(const Row& row) {
    uint32_t key = row.id;

    uint32_t page_num;
    if (meta.ROWS_IN_LAST_PAGE >= Constants::ROWS_PER_PAGE) {
        page_num = pager->newPage();
        meta.LAST_DATA_PAGE = page_num;
        meta.ROWS_IN_LAST_PAGE = 0;
    } else {
        page_num = meta.LAST_DATA_PAGE;
    }

    RowLocation value = {page_num, meta.NUM_ROWS % Constants::ROWS_PER_PAGE * Constants::ROW_SIZE};

    void* page = pager->getPage(value.page_num);
    std::memcpy(static_cast<char*>(page) + value.offset, &row, Constants::ROW_SIZE);
    pager->markDirty(value.page_num);

    if (!tree->insert(key, value)) {
        return false;
    }
    meta.NUM_ROWS++;
    storeMetaData();

    // tree->printTree();
    return true;
}

void Table::selectAll() {
    // tree->printTree();
    Cursor* cursor = tree->begin();
    while (!cursor->isEnd()) {
        printRow(cursor->getRow());
        cursor->advance();
    }
    delete cursor;
}
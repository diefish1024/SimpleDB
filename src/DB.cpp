#include "DB.hpp"
#include <iostream>

DB::DB(const std::string& filename) : pager(nullptr), table(nullptr) {
    open(filename);
}

DB::~DB() {
    close();
}

void DB::open(const std::string& filename) {
    close();
    pager = new Pager(filename);
    table = new Table(filename, pager);
}

void DB::close() {
    if (table) {
        table->flush();
        delete table;
        table = nullptr;
    }
    if (pager) {
        delete pager;
        pager = nullptr;
    }
}

void DB::flush() {
    if (table) {
        table->flush();
    }
}

Table* DB::getTable() const {
    return table;
}

#ifndef TABLE_HPP
#define TABLE_HPP

#include "Row.hpp"
#include "Pager.hpp"
#include <vector>
#include <cstdint>

const uint32_t MAX_ROWS = (MAX_PAGES - 1) * ROWS_PER_PAGE;
const uint32_t METADATA_PAGE_NUM = 0;
const uint32_t METADATA_OFFSET = 0;
const uint32_t MAX_META_PAGES = 1;

class Cursor;

class Table {
friend class Cursor;
public:
    explicit Table(const std::string& filename, Pager* pager);
    ~Table();

    // bool insertRow(const Row& row);
    // void selectAll() const;
    
    void flush(); // Ensure all changes are written to disk

    Cursor* start();
    Cursor* end();
    
private:
    struct MetaData {
        uint32_t NUM_ROWS;
    } meta;
    static const uint32_t METADATA_SIZE = sizeof(MetaData);

    void loadMetaData();
    void storeMetaData();

    Pager* pager;
};

#endif // TABLE_HPP
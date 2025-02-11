#ifndef TABLE_HPP
#define TABLE_HPP

#include <vector>
#include <cstdint>
#include "Row.hpp"
#include "Pager.hpp"

const uint32_t MAX_ROWS = 10000;
const uint32_t METADATA_PAGE_NUM = 0;
const uint32_t METADATA_OFFSET = 0;
const uint32_t MAX_META_PAGES = 1;

class Table {
public:

explicit Table(const std::string& filename, Pager* pager);
~Table();

    bool insertRow(const Row& row);
    void selectAll() const;
    
    void flush(); // Ensure all changes are written to disk
    
private:
    struct MetaData { // Sotred at Page 0
        uint32_t NUM_ROWS;
    } meta;
    static const uint32_t METADATA_SIZE = sizeof(MetaData);

    void loadMetaData();
    void storeMetaData();

    Pager* pager;
    uint32_t num_rows;
};

#endif // TABLE_HPP
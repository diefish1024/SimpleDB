#ifndef TABLE_HPP
#define TABLE_HPP

#include "Row.hpp"
#include "Pager.hpp"
#include "BPlusTree.hpp"
#include <vector>
#include <cstdint>

const uint32_t MAX_ROWS = 10000;
const uint32_t METADATA_PAGE_NUM = 0;
const uint32_t METADATA_OFFSET = 0;
const uint32_t MAX_META_PAGES = 1;
const uint32_t ROOT_PAGE_NUM = 1;

class Cursor;

class Table {
friend class Cursor;
public:
    explicit Table(Pager* pager);
    ~Table();
    
    void flush(); // Ensure all changes are written to disk

    bool insertRow(const Row& row);
    void selectAll();
    
private:
    #pragma pack(push, 1)
    struct MetaData {
        uint32_t NUM_ROWS;
        uint32_t LAST_DATA_PAGE;
        uint32_t ROWS_IN_LAST_PAGE;
    } meta;
    #pragma pack(pop)
    static const uint32_t METADATA_SIZE = sizeof(MetaData);

    void loadMetaData();
    void storeMetaData();

    Pager* pager;
    BPlusTree* tree;
};

#endif // TABLE_HPP
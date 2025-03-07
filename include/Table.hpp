#ifndef TABLE_HPP
#define TABLE_HPP

#include "Row.hpp"
#include "Pager.hpp"
#include "BPlusTree.hpp"
#include "Constants.hpp"
#include <vector>
#include <cstdint>

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
        uint32_t ROOT_PAGE_NUM;
    } meta;
    #pragma pack(pop)
    static const uint32_t METADATA_SIZE = sizeof(MetaData);

    void loadMetaData();
    void storeMetaData();

    Pager* pager;
    BPlusTree* tree;
};

#endif // TABLE_HPP
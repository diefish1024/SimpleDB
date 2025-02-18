#ifndef PAGER_HPP
#define PAGER_HPP

#include "Row.hpp"
#include <cstdint>
#include <fstream>

const uint32_t MAX_PAGES = 1000;
const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;

struct RowLocation {
    uint32_t page_num;
    uint32_t offset;

    bool isValid() const { return page_num != INVALID_PAGE; }

    static constexpr uint32_t INVALID_PAGE = UINT32_MAX;
};

class Pager {
public:
    std::fstream file;
    uint32_t file_length;
    void* pages[MAX_PAGES];

    explicit Pager(const std::string& filename);
    ~Pager();

    void* getPage(uint32_t page_num); // Read a page from disk
    Row getRow(const RowLocation& loc);
    void flush(uint32_t page_num); // Write a page to disk
    void markDirty(uint32_t page_num); // Mark a page as dirty
    
private:
    bool dirty_pages[MAX_PAGES];
};

#endif // PAGER_HPP
#ifndef PAGER_HPP
#define PAGER_HPP

#include "Row.hpp"
#include "Constants.hpp"
#include <cstdint>
#include <fstream>

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
    void** pages;
    uint32_t tot_pages;

    explicit Pager(const std::string& filename);
    ~Pager();

    void* getPage(uint32_t page_num); // Read a page from disk
    uint32_t newPage(); // Allocate a new page
    Row getRow(const RowLocation& loc);
    void flush(uint32_t page_num); // Write a page to disk
    void markDirty(uint32_t page_num); // Mark a page as dirty
    
private:
    bool* dirty_pages;
};

#endif // PAGER_HPP
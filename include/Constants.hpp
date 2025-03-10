#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>

namespace Constants {
    // Row
    constexpr  uint32_t COLUMN_USERNAME_MAX_LENGTH = 32;
    extern const uint32_t ROW_SIZE;
    constexpr uint32_t COLUMN_EMAIL_MAX_LENGTH = 255;
    
    // Table
    constexpr uint32_t MAX_ROWS = 10000;
    constexpr uint32_t METADATA_PAGE_NUM = 0;
    constexpr uint32_t METADATA_OFFSET = 0;
    constexpr uint32_t MAX_META_PAGES = 1;
    // constexpr uint32_t ROOT_PAGE_NUM = 1;
    
    // Pager
    constexpr uint32_t PAGE_SIZE = 4096;
    extern const uint32_t ROWS_PER_PAGE;
    extern const uint32_t MAX_PAGES;

    // BPlusTree
    constexpr uint32_t ORDER = 255;
}

#endif // CONSTANTS_HPP
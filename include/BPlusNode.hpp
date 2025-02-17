#ifndef BPLUSNODE_HPP
#define BPLUSNODE_HPP

#include "Row.hpp"
#include <cstdint>

const int ORDER = 255;

struct RowLocation {
    uint32_t page_num;
    uint32_t offset;
};

struct BPlusNode {
    bool is_leaf;
    uint32_t num_keys;
    uint32_t parent;
    uint32_t next;
    uint32_t prev;

    int keys[ORDER - 1];
    
    union {
        uint32_t children[ORDER];
        RowLocation values[ORDER];
    };
};

#endif // BPLUSNODE_HPP
#ifndef BPLUSNODE_HPP
#define BPLUSNODE_HPP

#include "Row.hpp"
#include "Pager.hpp"
#include <cstdint>

const int ORDER = 255;

struct BPlusNode {
    bool is_leaf;
    uint32_t num_keys;
    uint32_t parent;
    uint32_t next;
    uint32_t prev;

    int keys[ORDER - 1];
    
    union {
        uint32_t children[ORDER]; // page numbers of children
        RowLocation values[ORDER]; // values(locs) of leaf nodes
    };
};

#endif // BPLUSNODE_HPP
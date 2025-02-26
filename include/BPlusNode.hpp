#ifndef BPLUSNODE_HPP
#define BPLUSNODE_HPP

#include "Row.hpp"
#include "Pager.hpp"
#include "Constants.hpp"
#include <cstdint>

struct BPlusNode {
    bool is_leaf;
    uint32_t num_keys;
    uint32_t parent;
    uint32_t next;
    uint32_t prev;

    uint32_t page_num;

    int keys[Constants::ORDER- 1];
    
    union {
        uint32_t children[Constants::ORDER]; // page numbers of children
        RowLocation values[Constants::ORDER]; // values(locs) of leaf nodes
    };
};

#endif // BPLUSNODE_HPP
#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "Pager.hpp"

class BPlusTree;

class Cursor {
    public:
        Cursor() = delete;
        
        void advance();
        bool end_of_table() const { return is_end; }
    
        RowLocation currentLocation() const;
        Row getRow() const;
    
    private:
        friend class BPlusTree;
        
        Cursor(BPlusTree* tree, uint32_t page_num, uint32_t key_idx, bool is_end);
    
        BPlusTree* tree;
        uint32_t current_page;
        uint32_t current_key_idx;
        bool is_end = false;
    };

#endif // CURSOR_HPP
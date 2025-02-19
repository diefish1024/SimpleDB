#ifndef BPLUSTREE_HPP
#define BPLUSTREE_HPP

#include "BPlusNode.hpp"
#include "Cursor.hpp"

class BPlusTree {
public:
    BPlusTree(Pager* pager, uint32_t root_page_num);
    ~BPlusTree();

    Cursor* find(int key);
    bool insert(int key, const RowLocation& value);
    void remove(int key);

    Cursor* begin();
    Cursor* end();

    BPlusNode* getNode(uint32_t page_num);
    Pager* getPager() { return pager; }

private:
    BPlusNode* root;
    Pager* pager;

    void splitChild(BPlusNode* x, uint32_t child_idx);
};
#endif // BPLUSTREE_HPP
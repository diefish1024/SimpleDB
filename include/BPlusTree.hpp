#ifndef BPLUSTREE_HPP
#define BPLUSTREE_HPP

#include "BPlusNode.hpp"
#include "Cursor.hpp"

class BPlusTree {
public:
    BPlusTree(const std::string& filename);
    ~BPlusTree();

    Cursor* find(int key);
    void insert(int key, const RowLocation& value);
    void remove(int key);

    Cursor* begin();
    Cursor* end();

private:
    void splitNode(uint32_t page_num);
    void mergeNodes(uint32_t page_num, uint32_t child_page);
    void redistributeNodes(uint32_t page_num, uint32_t child_page);
};

#endif // BPLUSTREE_HPP
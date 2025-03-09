#ifndef BPLUSTREE_HPP
#define BPLUSTREE_HPP

#include "BPlusNode.hpp"
#include "Cursor.hpp"

extern bool isDebugMode;

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
    BPlusNode* initNewNode(uint32_t page_num);

    void printTree();

    uint32_t getRootPageNum() const { return root->page_num; }

private:
    BPlusNode* root;
    Pager* pager;

    void insertNonFull(BPlusNode* node, int key, uint32_t childPageNum);
    void splitNode(BPlusNode* node, BPlusNode* parent);
    void splitRoot(BPlusNode* oldRoot, int promotedKey, BPlusNode* newNode);

    void printNode(BPlusNode* node, int indent);
};
#endif // BPLUSTREE_HPP
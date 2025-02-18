#include "BPlusTree.hpp"
#include <iostream>
#include <algorithm>

BPlusNode* BPlusTree::getNode(uint32_t page_num) {
    return static_cast<BPlusNode*>(pager->getPage(page_num));
}

BPlusTree::BPlusTree(Pager* pager, uint32_t root_page_num)
    : pager(pager) {
    root = getNode(root_page_num);
}

Cursor* BPlusTree::find(int key) {
    BPlusNode* node = root;
    while (!node->is_leaf) {
        uint32_t child_index =
            std::upper_bound(node->keys, node->keys + node->num_keys, key) - node->keys;
        node = getNode(node->children[child_index]);
    }
    auto pos = std::lower_bound(node->keys, node->keys + node->num_keys, key);
    if (pos == node->keys + node->num_keys || *pos != key) {
        return new Cursor(this, 0, 0, true); // Invalid cursor
    }
    int idx = pos - node->keys;
    return new Cursor(this, node->values[idx].page_num, idx, false);
}

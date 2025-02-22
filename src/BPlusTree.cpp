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

BPlusTree::~BPlusTree() {
    delete root;
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

bool BPlusTree::insert(int key, const RowLocation& value) {
    BPlusNode* x = root;
    BPlusNode* y = nullptr;

    // DEBUG
    // std::cout << "Root page num: " << root->page_num << std::endl;

    while (!x->is_leaf) {
        uint32_t child_index =
            std::upper_bound(x->keys, x->keys + x->num_keys, key) - x->keys;
        y = x;
        x = getNode(x->children[child_index]);
    }
    auto pos = std::lower_bound(x->keys, x->keys + x->num_keys, key);
    if (pos != x->keys + x->num_keys && *pos == key) {
        std::cerr << "Duplicate key: " << key << std::endl;
        return false;
    }
    int idx = pos - x->keys;
    for (int i = x->num_keys; i > idx; --i) {
        x->keys[i] = x->keys[i - 1];
        x->values[i] = x->values[i - 1];
    }
    x->keys[idx] = key;
    x->values[idx] = value;
    x->num_keys++;
    pager->markDirty(x->page_num);
    if (x->num_keys == ORDER - 1) {
        uint32_t child_idx =
            std::upper_bound(y->keys, y->keys + y->num_keys, key) - y->keys;
        splitChild(y, child_idx);
    }
    return true;
}

void BPlusTree::splitChild(BPlusNode* x, uint32_t child_idx) {
    uint32_t tot_pages = pager->file_length / PAGE_SIZE;
    BPlusNode* z = getNode(tot_pages + 1);
    BPlusNode* y = getNode(x->children[child_idx]);
    z->is_leaf = y->is_leaf;
    z->num_keys = ORDER / 2;
    z->parent = x->parent;
    for (int i = 0; i < z->num_keys; ++i) {
        z->keys[i] = y->keys[i + ORDER / 2];
        z->values[i] = y->values[i + ORDER / 2];
    }
    if (!y->is_leaf) {
        for (int i = 0; i <= z->num_keys; ++i) {
            z->children[i] = y->children[i + ORDER / 2];
            BPlusNode* child = getNode(z->children[i]);
            child->parent = tot_pages + 1;
            pager->markDirty(z->children[i]);
        }
    }
    y->num_keys = ORDER / 2;
    for (int i = x->num_keys; i > child_idx; --i) {
        x->keys[i] = x->keys[i - 1];
        x->children[i + 1] = x->children[i];
    }
    x->keys[child_idx] = y->keys[ORDER / 2 - 1];
    x->children[child_idx + 1] = tot_pages + 1;
    x->num_keys++;
    pager->markDirty(x->parent);
}

Cursor* BPlusTree::begin() {
    BPlusNode* node = root;
    while (!node->is_leaf) {
        node = getNode(node->children[0]);
    }
    return new Cursor(this, node->page_num, 0, false);
}

Cursor* BPlusTree::end() {
    BPlusNode* node = root;
    while (!node->is_leaf) {
        node = getNode(node->children[node->num_keys]);
    }
    return new Cursor(this, node->page_num, node->num_keys, true);
}


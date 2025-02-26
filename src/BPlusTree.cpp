#include "BPlusTree.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cstring>

BPlusNode* BPlusTree::getNode(uint32_t page_num) {
    return static_cast<BPlusNode*>(pager->getPage(page_num));
}

BPlusNode* BPlusTree::initNewNode(uint32_t page_num) {
    BPlusNode* node = getNode(page_num);

    node->is_leaf = true;
    node->num_keys = 0;
    node->parent = 0;
    node->next = 0;
    node->prev = 0;
    node->page_num = page_num;
    std::memset(node->keys, 0, sizeof(node->keys));
    std::memset(node->children, 0, sizeof(node->children));
    std::memset(node->values, 0, sizeof(node->values));

    pager->markDirty(page_num);
    return node;
}

BPlusTree::BPlusTree(Pager* pager, uint32_t root_page_num)
    : pager(pager) {
    root = getNode(root_page_num);
}

BPlusTree::~BPlusTree() = default; // don't need to delete root

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
    if (x->num_keys == Constants::ORDER - 1) {
        uint32_t child_idx =
            std::upper_bound(y->keys, y->keys + y->num_keys, key) - y->keys;
        splitChild(y, child_idx);
    }
    return true;
}

void BPlusTree::splitChild(BPlusNode* x, uint32_t child_idx) {
    uint32_t tot_pages = pager->file_length / Constants::PAGE_SIZE;
    BPlusNode* z = getNode(tot_pages);
    BPlusNode* y = getNode(x->children[child_idx]);
    z->is_leaf = y->is_leaf;
    z->num_keys = Constants::ORDER / 2;
    z->parent = x->parent;
    for (int i = 0; i < z->num_keys; ++i) {
        z->keys[i] = y->keys[i + Constants::ORDER / 2];
        z->values[i] = y->values[i + Constants::ORDER / 2];
    }
    if (!y->is_leaf) {
        for (int i = 0; i <= z->num_keys; ++i) {
            z->children[i] = y->children[i + Constants::ORDER / 2];
            BPlusNode* child = getNode(z->children[i]);
            child->parent = tot_pages;
            pager->markDirty(z->children[i]);
        }
    }
    y->num_keys = Constants::ORDER / 2;
    for (int i = x->num_keys; i > child_idx; --i) {
        x->keys[i] = x->keys[i - 1];
        x->children[i + 1] = x->children[i];
    }
    x->keys[child_idx] = y->keys[Constants::ORDER / 2 - 1];
    x->children[child_idx + 1] = tot_pages;
    x->num_keys++;
    pager->markDirty(x->page_num);
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

void BPlusTree::printTree() {
    std::cout << "B+ Tree Structure:\n";
    printNode(root, 0);
}

void BPlusTree::printNode(BPlusNode* node, int indent) {
    if (node == nullptr)
        return;

    std::cout << std::setw(indent * 4) << "" << "Page: " << node->page_num
              << ", Leaf: " << (node->is_leaf ? "Yes" : "No")
              << ", Keys: [";
    for (int i = 0; i < node->num_keys; ++i) {
        std::cout << node->keys[i];
        if (i < node->num_keys - 1)
            std::cout << ", ";
    }
    std::cout << "], Children: [";
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; ++i) {
            std::cout << node->children[i];
            if (i < node->num_keys)
                std::cout << ", ";
        }
    } else {
        for (int i = 0; i < node->num_keys; ++i) {
            std::cout << "Value@" << node->values[i].page_num << ":" << node->values[i].offset;
            if (i < node->num_keys - 1)
                std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;

    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; ++i) {
            BPlusNode* child = getNode(node->children[i]);
            if (child != nullptr) {
                printNode(child, indent + 1);
            } else {
                std::cout << std::setw((indent + 1) * 4) << "" << "Child Page: " << node->children[i] << " is nullptr" << std::endl;
            }
        }
    }
}
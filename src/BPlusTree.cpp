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
        splitNode(x, y);
    }
    return true;
}

void BPlusTree::splitNode(BPlusNode* node, BPlusNode* parent) {
    int order = Constants::ORDER;
    int splitPoint = order / 2;

    BPlusNode* newNode = initNewNode(pager->newPage()); // Use initNewNode for proper initialization
    newNode->is_leaf = node->is_leaf;
    newNode->parent = node->parent; // Inherit parent from the node being split
    newNode->num_keys = 0;

    if (node->is_leaf) {
        // Leaf node split
        for (int i = splitPoint; i < order - 1; ++i) {
            newNode->keys[newNode->num_keys] = node->keys[i];
            newNode->values[newNode->num_keys] = node->values[i];
            newNode->num_keys++;
        }
        node->num_keys = splitPoint;

        newNode->prev = node->page_num;
        newNode->next = node->next;
        node->next = newNode->page_num;
        if (newNode->next != 0) {
            BPlusNode* nextNode = getNode(newNode->next);
            nextNode->prev = newNode->page_num;
            pager->markDirty(nextNode->page_num);
        }

        pager->markDirty(node->page_num);
        pager->markDirty(newNode->page_num);

        int promotedKey = newNode->keys[0];

        // Handle root split for leaf node (if root is leaf and full) - unlikely but possible for very small trees
        if (node->page_num == root->page_num && parent == nullptr) {
            splitRoot(node, promotedKey, newNode);
        } else if (parent != nullptr) {
            insertNonFull(parent, promotedKey, newNode->page_num); // Insert into parent
        } else {
            // This case should ideally not happen in standard B+ tree insertion after finding leaf.
            // However, if root was originally a leaf and got split, it should be handled by splitRoot.
            // If we reach here, it might indicate a logic error in calling splitNode.
            std::cerr << "Warning: splitNode called on root leaf without proper root split handling." << std::endl;
        }


    } else {
        // Non-leaf node split
        for (int i = splitPoint; i < order - 1; ++i) {
            newNode->keys[newNode->num_keys] = node->keys[i];
            newNode->children[newNode->num_keys + 1] = node->children[i + 1];
            newNode->num_keys++;
            BPlusNode* child = getNode(node->children[i + 1]);
            if (child != nullptr) {
                child->parent = newNode->page_num;
                pager->markDirty(child->page_num);
            }
        }
        newNode->children[0] = node->children[splitPoint];
        BPlusNode* firstChild = getNode(node->children[splitPoint]);
        if (firstChild != nullptr) {
            firstChild->parent = newNode->page_num;
            pager->markDirty(firstChild->page_num);
        }
        node->num_keys = splitPoint - 1;

        pager->markDirty(node->page_num);
        pager->markDirty(newNode->page_num);

        int promotedKey = node->keys[splitPoint - 1];

        // Handle root split for non-leaf node
        if (node->page_num == root->page_num && parent == nullptr) {
            splitRoot(node, promotedKey, newNode);
        } else if (parent != nullptr) {
            insertNonFull(parent, promotedKey, newNode->page_num); // Insert into parent
        } else {
            // Similar warning as in leaf split, but for non-leaf root split without parent.
            std::cerr << "Warning: splitNode called on root non-leaf without proper root split handling." << std::endl;
        }
    }
}


void BPlusTree::splitRoot(BPlusNode* oldRoot, int promotedKey, BPlusNode* newNode) {
    BPlusNode* newRoot = initNewNode(pager->newPage()); // Initialize new root node
    newRoot->is_leaf = false;
    newRoot->num_keys = 1;
    newRoot->keys[0] = promotedKey;
    newRoot->children[0] = oldRoot->page_num;
    newRoot->children[1] = newNode->page_num;
    oldRoot->parent = newRoot->page_num;
    newNode->parent = newRoot->page_num;

    pager->markDirty(newRoot->page_num);
    pager->markDirty(oldRoot->page_num);
    pager->markDirty(newNode->page_num);

    root = newRoot; // Update root to the new root
}

void BPlusTree::insertNonFull(BPlusNode* node, int key, uint32_t childPageNum) {
    if (isDebugMode) {
        std::cout << "[DEBUG] insertNonFull: node->page_num=" << node->page_num
                  << ", node->is_leaf=" << (node->is_leaf ? "Yes" : "No")
                  << ", key=" << key << ", childPageNum=" << childPageNum << std::endl;
    }
    if (isDebugMode) {
        printTree();
    }
    int i = node->num_keys - 1;
    if (node->is_leaf) {
        // Should not be called for leaf nodes after split. SplitNode handles leaf split and parent insertion.
        // insertNonFull is for inserting into non-leaf nodes.
        std::cerr << "Error: insertNonFull called for leaf node which should not happen after split." << std::endl;
        return;

    } else {
        // Insert into non-leaf node
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++; // i is now the correct child index
        BPlusNode* child = getNode(node->children[i]);
        if (child->num_keys == Constants::ORDER - 1) {
            splitNode(child, node); // Split child, parent is current node
             // After split, the middle key is promoted to 'node', and child is split.
            // We need to decide which of the two children of the split node to descend into.
            if (key > node->keys[i]) { // if key > promoted key (node->keys[i] after insertion in splitNode)
                i++; // go to the right child (index i+1 after insertion in splitNode)
            }
            // else go to the left child (index i) which is default.
        } else {
            // Recursively insert into the (possibly newly split) child
            insertNonFull(getNode(node->children[i]), key, childPageNum); // Pass down original childPageNum
        }
    }
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
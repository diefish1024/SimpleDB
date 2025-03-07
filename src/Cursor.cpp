#include "Cursor.hpp"
#include "BPlusTree.hpp"
#include "Pager.hpp"
#include "Table.hpp"

Cursor::Cursor(BPlusTree* tree, uint32_t page_num, uint32_t key_idx, bool is_end)
    : tree(tree), current_page(page_num), current_key_idx(key_idx), is_end(is_end) {
    if (page_num >= Constants::MAX_META_PAGES) {
        BPlusNode* node = tree->getNode(page_num);
        if (node->num_keys == 0) this->is_end = true;
    }
}

void Cursor::advance() {
    if (is_end) return;

    BPlusNode* node = tree->getNode(current_page);
    current_key_idx++;
    
    if (current_key_idx >= node->num_keys) {
        if (node->next == 0) {
            is_end = true;
        } else {
            current_page = node->next;
            current_key_idx = 0;
        }
    }
}

RowLocation Cursor::currentLocation() const {
    if (is_end) return {RowLocation::INVALID_PAGE, 0};
    BPlusNode* node = tree->getNode(current_page);
    return node->values[current_key_idx];
}

Row Cursor::getRow() const {
    if (is_end) return {};
    RowLocation loc = currentLocation();
    return tree->getPager()->getRow(loc);
}
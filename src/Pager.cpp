#include "Pager.hpp"
#include <string>
#include <iostream>
#include <stdexcept>
#include <cstring>

Pager::Pager(const std::string& filename) {
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        file.open(filename, std::ios::out | std::ios::binary);
        file.close();
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    }
    file.seekg(0, std::ios::end);
    file_length = file.tellg();
    file.clear();

    pages = new void*[Constants::MAX_PAGES];
    dirty_pages = new bool[Constants::MAX_PAGES];
    
    for (uint32_t i = 0; i < Constants::MAX_PAGES; i++) {
        pages[i] = nullptr;
        dirty_pages[i] = false;
    }

    tot_pages = file_length / Constants::PAGE_SIZE;

    for (uint32_t i = 0; i < Constants::MAX_PAGES; i++) {
        pages[i] = nullptr;
        dirty_pages[i] = false;
    }
}

Pager::~Pager() {
    for (uint32_t i = 0; i < Constants::MAX_PAGES; i++) {
        if (pages[i] != nullptr) {
            delete[] static_cast<char*>(pages[i]);
            pages[i] = nullptr;
        }
    }
    file.close();

    delete[] pages;
    delete[] dirty_pages;
}

void* Pager::getPage(uint32_t page_num) {
    // std::cout << "Getting page " << page_num << std::endl;
    if (page_num >= Constants::MAX_PAGES) {
        throw std::runtime_error("Page number out of bounds");
    }

    // std::cout << "Total pages: " << tot_pages << std::endl;

    if (pages[page_num] == nullptr) {
        pages[page_num] = new char[Constants::PAGE_SIZE];
        if (tot_pages > page_num) { // Page exists in file
            file.seekg(page_num * Constants::PAGE_SIZE);
            file.read(static_cast<char*>(pages[page_num]), Constants::PAGE_SIZE);
            if (file.fail()) {
                std::memset(pages[page_num], 0, Constants::PAGE_SIZE);
                file.clear();
            }
        } else {
            memset(pages[page_num], 0, Constants::PAGE_SIZE);
            file_length = Constants::PAGE_SIZE * (page_num + 1);
            tot_pages = page_num + 1;
        }
    }
    return pages[page_num];
}

uint32_t Pager::newPage() {
    if (tot_pages >= Constants::MAX_PAGES) {
        throw std::runtime_error("Maximum number of pages reached.");
    }

    tot_pages++;

    // Extend file size to accommodate the new page if necessary
    uint32_t required_file_length = tot_pages * Constants::PAGE_SIZE;
    if (file_length < required_file_length) {
        file.clear();
        file.seekp(required_file_length - 1);
        file.write("\0", 1); // Extend file by one byte to ensure space for the new page
        file_length = required_file_length; // Update file_length
    }

    return tot_pages - 1;
}

Row Pager::getRow(const RowLocation& loc) {
    if (!loc.isValid()) {
        return {};
    }
    void* page = getPage(loc.page_num);
    return *reinterpret_cast<Row*>(static_cast<char*>(page) + loc.offset);
}

void Pager::flush(uint32_t page_num) {
    if (pages[page_num] == nullptr || !dirty_pages[page_num]) {
        return;
    }

    // Ensure file is open
    if (!file.is_open()) {
        throw std::runtime_error("File is not open");
    }
    file.clear();
    file.seekp(page_num * Constants::PAGE_SIZE);
    if (file.fail()) {
        throw std::runtime_error("Seek failed at page " + std::to_string(page_num));
    }

    // Write the page data
    if (file.write(static_cast<char*>(pages[page_num]), Constants::PAGE_SIZE)) {
        dirty_pages[page_num] = false;
    } else {
        throw std::runtime_error("Error writing to file");
    }
}

void Pager::markDirty(uint32_t page_num) {
    dirty_pages[page_num] = true;
}
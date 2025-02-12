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

    for (uint32_t i = 0; i < MAX_PAGES; i++) {
        pages[i] = nullptr;
        dirty_pages[i] = false;
    }
}

Pager::~Pager() {
    for (uint32_t i = 0; i < MAX_PAGES; i++) {
        if (pages[i] != nullptr) {
            flush(i);
            delete[] static_cast<char*>(pages[i]);
            pages[i] = nullptr;
        }
    }
    file.close();
}

void* Pager::getPage(uint32_t page_num) {
    if (page_num >= MAX_PAGES) {
        throw std::runtime_error("Page number out of bounds");
    }

    if (pages[page_num] == nullptr) {
        pages[page_num] = new char[PAGE_SIZE];
        uint32_t tot_pages = file_length / PAGE_SIZE;
        if (file_length % PAGE_SIZE) {
            tot_pages++;
        }
        if (tot_pages >= page_num) { // Page exists in file
            file.seekg(page_num * PAGE_SIZE);
            file.read(static_cast<char*>(pages[page_num]), PAGE_SIZE);
            if (file.fail()) {
                std::memset(pages[page_num], 0, PAGE_SIZE);
                file.clear();
            }
        } else {
            memset(pages[page_num], 0, PAGE_SIZE);
            file_length = PAGE_SIZE * (page_num + 1);
        }
    }
    return pages[page_num];
}

void Pager::flush(uint32_t page_num) { // ToDo: fix some bugs here
    if (pages[page_num] == nullptr || !dirty_pages[page_num]) {
        return;
    }
    // std::cout << "Flushing page " << page_num << std::endl;

    // Ensure file is open
    if (!file.is_open()) {
        throw std::runtime_error("File is not open");
    }

    // Seek to the end to get the file size
    // file.clear();
    // file.seekp(0, std::ios::end);
    // uint32_t file_size = file.tellp();
    // std::cout << "File size: " << file_size << std::endl;

    // Ensure file size is large enough for the page
    // uint32_t required_size = (page_num + 1) * PAGE_SIZE;
    // if (file_size < required_size) {
    //     // Extend the file if necessary
    //     file.clear();
    //     file.seekp(required_size - 1);
    //     file.write("\0", 1);  // Write a single byte to extend the file
    //     file.flush();
    // }

    // Now we can safely seek to the target page position
    file.clear();
    file.seekp(page_num * PAGE_SIZE);
    if (file.fail()) {
        throw std::runtime_error("Seek failed at page " + std::to_string(page_num));
    }

    // std::cout << "File write pointer is at: " << file.tellp() << std::endl;

    // Write the page data
    if (file.write(static_cast<char*>(pages[page_num]), PAGE_SIZE)) {
        dirty_pages[page_num] = false;
    } else {
        throw std::runtime_error("Error writing to file");
    }
}


void Pager::markDirty(uint32_t page_num) {
    dirty_pages[page_num] = true;
}
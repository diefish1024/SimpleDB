#ifndef DB_HPP
#define DB_HPP

#include <string>
#include "Pager.hpp"
#include "Table.hpp"

class DB {
private:
    Pager* pager;
    Table* table;

public:
    explicit DB(const std::string& filename);
    ~DB();

    void open(const std::string& filename); // Open the database
    void close(); // Close the database and flush pages to disk
    void flush(); // Flush all pages to disk
    Table* getTable() const; // Return the table object
};

#endif // DB_HPP

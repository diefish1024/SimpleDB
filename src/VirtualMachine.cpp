#include "VirtualMachine.hpp"
#include "Statement.hpp"
#include "Table.hpp"
#include "Cursor.hpp"
#include <iostream>

VirtualMachine::VirtualMachine(DB* db) : db(db) {}

ExcuteResult VirtualMachine::run(const Statement& statement) {
    switch (statement.type) {
        case StatementType::STATEMENT_INSERT:
            return executeInsert(statement);
        case StatementType::STATEMENT_SELECT:
            return executeSelect(statement);
    }
    return ExcuteResult::EXECUTE_UNKNOWN;
}

ExcuteResult VirtualMachine::executeInsert(const Statement& statement) {
    Table* table = db->getTable();
    Cursor* cursor = table->end();
    
    if (cursor->row_num >= MAX_ROWS) {
        return ExcuteResult::EXECUTE_TABLE_FULL;
    }

    cursor->insert(statement.row_to_insert);
    delete cursor;
    return ExcuteResult::EXECUTE_SUCCESS;
}

ExcuteResult VirtualMachine::executeSelect(const Statement& statement) {
    Table* table = db->getTable();
    Cursor* cursor = table->start();
    while (!cursor->end_of_table) {
        Row row = cursor->getRow();
        printRow(row);
        cursor->advance();
    }
    return ExcuteResult::EXECUTE_SUCCESS;
}
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
    Row row = statement.row_to_insert;
}

ExcuteResult VirtualMachine::executeSelect(const Statement& statement) {
}
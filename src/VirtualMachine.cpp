#include "VirtualMachine.hpp"
#include "Statement.hpp"
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
    return db->getTable()->insertRow(statement.row_to_insert) ? 
        ExcuteResult::EXECUTE_SUCCESS : ExcuteResult::EXECUTE_TABLE_FULL;
}

ExcuteResult VirtualMachine::executeSelect(const Statement& statement) {
    db->getTable()->selectAll();
    return ExcuteResult::EXECUTE_SUCCESS;
}
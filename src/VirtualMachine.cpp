#include "VirtualMachine.hpp"
#include "Statement.hpp"
#include <iostream>

ExcuteResult VirtualMachine::run(const Statement& statement) {
    switch (statement.type) {
        case StatementType::INSERT:
            return executeInsert(statement);
        case StatementType::SELECT:
            return executeSelect(statement);
    }
    return ExcuteResult::EXECUTE_UNKNOWN;
}

ExcuteResult VirtualMachine::executeInsert(const Statement& statement) {
    return table->insertRow(statement.row_to_insert) ? 
        ExcuteResult::EXECUTE_SUCCESS : ExcuteResult::EXECUTE_TABLE_FULL;
}

ExcuteResult VirtualMachine::executeSelect(const Statement& statement) {
    table->selectAll();
    return ExcuteResult::EXECUTE_SUCCESS;
}
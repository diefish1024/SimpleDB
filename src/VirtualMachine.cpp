#include "VirtualMachine.hpp"
#include "Statement.hpp"
#include <iostream>

VirtualMachine::VirtualMachine() : table() {}

void VirtualMachine::run(const Statement& statement) {
    switch (statement.type) {
        case StatementType::INSERT:
            executeInsert(statement);
            break;
        case StatementType::SELECT:
            executeSelect(statement);
            break;
    }
}

void VirtualMachine::executeInsert(const Statement& statement) {
    table.insertRow(statement.row_to_insert);
}

void VirtualMachine::executeSelect(const Statement& statement) {
    table.selectAll();
}
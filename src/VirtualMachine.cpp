#include "VirtualMachine.hpp"
#include "Statement.hpp"
#include <iostream>

void VirtualMachine::run(const Statement& statement) {
    switch (statement.type) {
        case StatementType::INSERT:
            executeInsert();
            break;
        case StatementType::SELECT:
            executeSelect();
            break;
    }
}

void VirtualMachine::executeInsert() {
    std::cout << "This is where we would do an insert." << std::endl;
}

void VirtualMachine::executeSelect() {
    std::cout << "This is where we would do a select." << std::endl;
}
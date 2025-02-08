#ifndef VIRTUALMACHINE_HPP
#define VIRTUALMACHINE_HPP

#include "Statement.hpp"
#include "Table.hpp"

class VirtualMachine {
public:
    VirtualMachine();
    VirtualMachine(const VirtualMachine&) = delete;
    ~VirtualMachine() = default;

    void run(const Statement& statement);
private:
    void executeInsert(const Statement& statement);
    void executeSelect(const Statement& statement);
    
    Table table;
};

#endif // VIRTUALMACHINE_HPP
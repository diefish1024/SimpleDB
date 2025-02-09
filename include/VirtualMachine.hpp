#ifndef VIRTUALMACHINE_HPP
#define VIRTUALMACHINE_HPP

#include "Statement.hpp"
#include "Table.hpp"

enum class ExcuteResult {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL,
    EXECUTE_UNKNOWN
};

class VirtualMachine {
public:
    VirtualMachine(Table* table) : table(table) {};
    VirtualMachine(const VirtualMachine&) = delete;
    ~VirtualMachine() = default;

    ExcuteResult run(const Statement& statement);
private:
    ExcuteResult executeInsert(const Statement& statement);
    ExcuteResult executeSelect(const Statement& statement);
    
    Table* table;
};

#endif // VIRTUALMACHINE_HPP
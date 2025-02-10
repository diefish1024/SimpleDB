#ifndef VIRTUALMACHINE_HPP
#define VIRTUALMACHINE_HPP

#include "Statement.hpp"
#include "DB.hpp"

enum class ExcuteResult {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL,
    EXECUTE_UNKNOWN
};

class VirtualMachine {
public:
    VirtualMachine(DB* db);
    VirtualMachine(const VirtualMachine&) = delete;
    ~VirtualMachine() = default;

    ExcuteResult run(const Statement& statement);
private:
    ExcuteResult executeInsert(const Statement& statement);
    ExcuteResult executeSelect(const Statement& statement);
    
    DB* db;
};

#endif // VIRTUALMACHINE_HPP
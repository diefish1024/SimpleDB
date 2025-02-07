#ifndef VIRTUALMACHINE_HPP
#define VIRTUALMACHINE_HPP

#include "Statement.hpp"

class VirtualMachine {
public:
    VirtualMachine() = default;
    ~VirtualMachine() = default;

    void run(const Statement& statement);
private:
    void executeInsert();
    void executeSelect();
};

#endif // VIRTUALMACHINE_HPP
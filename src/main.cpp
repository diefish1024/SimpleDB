#include "InputBuffer.hpp"
#include "Statement.hpp"
#include "StatementParser.hpp"
#include "VirtualMachine.hpp"
#include "MetaCommand.hpp"
#include <iostream>
#include <string>

void printPrompt() {
    std::cout << "db > ";
}

int main() {
    InputBuffer input_buffer;
    VirtualMachine vm;
    while (true) {
        printPrompt();
        input_buffer.readInput();

        const std::string& input = input_buffer.getBuffer();

        if (!input.empty() && input[0] == '.') {
            auto metaResult = doMetaCommand(input);
            if (metaResult == MetaCommandResult::META_COMMAND_UNRECOGNIZED) {
                continue;
            }
        }
        else {
            Statement statement;
            auto prepResult = prepareStatement(input, statement);
            if (prepResult != PrepareResult::PREPARE_SUCCESS) {
                std::cerr << "Error: Failed to prepare statement." << std::endl;
                continue;
            }
            vm.run(statement);
        }
    }
    return 0;
}
#include "InputBuffer.hpp"
#include "Statement.hpp"
#include "StatementParser.hpp"
#include "VirtualMachine.hpp"
#include "MetaCommand.hpp"
#include <iostream>
#include <string>
#include <sstream>

void printPrompt() {
    std::cout << "db > ";
}

int main() {
    InputBuffer input_buffer;
    VirtualMachine vm(new Table());
    while (true) {
        printPrompt();
        input_buffer.readInput();

        const std::string& input = input_buffer.getBuffer();

        if (!input.empty() && input[0] == '.') {
            auto metaResult = doMetaCommand(input);
            switch (metaResult) {
                case MetaCommandResult::META_COMMAND_SUCCESS:
                    continue;
                case MetaCommandResult::META_COMMAND_UNRECOGNIZED:
                    std::cerr << "Unrecognized command: " << input << std::endl;
                    continue;
            }
        }
        else {
            Statement statement;
            auto prepResult = prepareStatement(input, statement);
            switch (prepResult) {
                case PrepareResult::PREPARE_SUCCESS:
                    break;
                case PrepareResult::PREPARE_SYNTAX_ERROR:
                    std::cerr << "Syntax error. Could not parse statement." << std::endl;
                    continue;
                case PrepareResult::PREPARE_NEGATIVE_ID:
                    std::cerr << "ID must be positive." << std::endl;
                    continue;
                case PrepareResult::REPARE_STRING_TOO_LONG:
                    std::cerr << "String is too long." << std::endl;
                    continue;
                case PrepareResult::PREPARE_UNRECOGNIZED: {
                    std::istringstream iss(input);
                    std::string token;
                    iss >> token;
                    std::cerr << "Unrecognized keyword at start of " << token << std::endl;
                    continue;
                }
            }

            auto executeResult = vm.run(statement);
            switch (executeResult) {
                case ExcuteResult::EXECUTE_SUCCESS:
                    std::cout << "Executed." << std::endl;
                    break;
                case ExcuteResult::EXECUTE_TABLE_FULL:
                    std::cerr << "Error: Table is full." << std::endl;
                    break;
            }
        }
    }
    return 0;
}
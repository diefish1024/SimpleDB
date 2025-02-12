#include "InputBuffer.hpp"
#include "Statement.hpp"
#include "StatementParser.hpp"
#include "VirtualMachine.hpp"
#include "MetaCommand.hpp"
#include "DB.hpp"
#include <iostream>
#include <string>
#include <sstream>

void printPrompt() {
    std::cout << "db > ";
}

void printConfig() {
    std::cout << "Row size = " << ROW_SIZE << " bytes" << std::endl;
    std::cout << "Rows per page = " << ROWS_PER_PAGE << std::endl;
    std::cout << "Page size = " << PAGE_SIZE << " bytes" << std::endl;
    std::cout << "Max pages = " << MAX_PAGES << std::endl;
    std::cout << "Max rows = " << MAX_ROWS << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--config") {
        printConfig();
        return 0;
    }

    if (argc < 2) {
        std::cerr << "Must supply a database filename." << std::endl;
        return 1;
    }

    InputBuffer input_buffer;

    std::string filename(argv[1]);
    DB db(filename);
    VirtualMachine vm(&db);

    while (true) {
        printPrompt();
        input_buffer.readInput();

        const std::string& input = input_buffer.getBuffer();

        if (!input.empty() && input[0] == '.') {
            auto metaResult = parseMetaCommand(input);
            switch (metaResult) {
                case MetaCommandResult::META_COMMAND_EXIT:
                    db.close();
                    return 0;
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
}
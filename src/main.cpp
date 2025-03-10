#include "InputBuffer.hpp"
#include "Statement.hpp"
#include "StatementParser.hpp"
#include "VirtualMachine.hpp"
#include "MetaCommand.hpp"
#include "DB.hpp"
#include "Constants.hpp"
#include <iostream>
#include <string>
#include <sstream>

bool isDebugMode = false;

void printPrompt() {
    std::cout << "db > ";
}

void printConfig() {
    std::cout << "--- Configuration ---" << std::endl;
    std::cout << "ROW_SIZE = " << Constants::ROW_SIZE << " bytes" << std::endl;
    std::cout << "ROWS_PER_PAGE = " << Constants::ROWS_PER_PAGE << std::endl;
    std::cout << "PAGE_SIZE = " << Constants::PAGE_SIZE << " bytes" << std::endl;
    std::cout << "MAX_PAGES = " << Constants::MAX_PAGES << std::endl;
    std::cout << "MAX_ROWS = " << Constants::MAX_ROWS << std::endl;
    std::cout << std::endl;

    std::cout << "COLUMN_USERNAME_MAX_LENGTH = " << Constants::COLUMN_USERNAME_MAX_LENGTH << " bytes" << std::endl;
    std::cout << "COLUMN_EMAIL_MAX_LENGTH = " << Constants::COLUMN_EMAIL_MAX_LENGTH << " bytes" << std::endl;
    std::cout << std::endl;

    std::cout << "METADATA_PAGE_NUM = " << Constants::METADATA_PAGE_NUM << std::endl;
    std::cout << "METADATA_OFFSET = " << Constants::METADATA_OFFSET << std::endl;
    std::cout << "MAX_META_PAGES = " << Constants::MAX_META_PAGES << std::endl;
    std::cout << std::endl;

    std::cout << "B+TREE ORDER = " << Constants::ORDER << std::endl;
    std::cout << "--- End Configuration ---" << std::endl;
}

int main(int argc, char* argv[]) {
    bool debugMode = false;

    std::string filename;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-debug") {
            debugMode = true;
        } else if (arg == "--show-config") {
            printConfig();
            return 0;
        } else if (filename.empty()) {
            filename = arg;
        } else {
            std::cerr << "Too many arguments provided." << std::endl;
            return 1;
        }
    }
    if (filename.empty()) {
        std::cerr << "Must supply a database filename." << std::endl;
        return 1;
    }
    if (debugMode) {
        isDebugMode = true;
        std::cout << "Debug mode is ON." << std::endl;
    }

    InputBuffer input_buffer;
    
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
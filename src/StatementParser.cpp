#include "Statement.hpp"
#include "StatementParser.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

PrepareResult prepareStatement(const std::string& input, Statement& statement) {
    std::istringstream iss(input);
    std::string token;
    iss >> token;

    if (token == "insert") {
        statement.type = StatementType::INSERT;
        return PrepareResult::PREPARE_SUCCESS;
    } else if (token == "select") {
        statement.type = StatementType::SELECT;
        return PrepareResult::PREPARE_SUCCESS;
    } else {
        return PrepareResult::PREPARE_UNRECOGNIZED;
    }
}


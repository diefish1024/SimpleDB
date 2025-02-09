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
        int id;
        std::string username;
        std::string email;
        if (!(iss >> id >> username >> email)) {
            return PrepareResult::PREPARE_SYNTAX_ERROR;
        }
        if (iss >> token) { // exam if there are more tokens
            return PrepareResult::PREPARE_SYNTAX_ERROR;
        }
        if (id < 0) {
            return PrepareResult::PREPARE_NEGATIVE_ID;
        }
        if (username.size() > COLUMN_USERNAME_MAX_LENGTH) {
            return PrepareResult::REPARE_STRING_TOO_LONG;
        }
        if (email.size() > COLUMN_EMAIL_MAX_LENGTH) {
            return PrepareResult::REPARE_STRING_TOO_LONG;
        }
        statement.row_to_insert = {id, username, email};
        return PrepareResult::PREPARE_SUCCESS;
    } else if (token == "select") {
        if (iss >> token) { // exam if there are more tokens
            return PrepareResult::PREPARE_SYNTAX_ERROR;
        }
        statement.type = StatementType::SELECT;
        return PrepareResult::PREPARE_SUCCESS;
    } else {
        return PrepareResult::PREPARE_UNRECOGNIZED;
    }
}


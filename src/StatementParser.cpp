#include "Statement.hpp"
#include "StatementParser.hpp"
#include "Constants.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>

PrepareResult prepareStatement(const std::string& input, Statement& statement) {
    std::istringstream iss(input);
    std::string token;
    iss >> token;

    if (token == "insert") {
        statement.type = StatementType::STATEMENT_INSERT;
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
        if (username.size() > Constants::COLUMN_USERNAME_MAX_LENGTH) {
            return PrepareResult::REPARE_STRING_TOO_LONG;
        }
        if (email.size() > Constants::COLUMN_EMAIL_MAX_LENGTH) {
            return PrepareResult::REPARE_STRING_TOO_LONG;
        }
        std::strncpy(statement.row_to_insert.username, username.c_str(), Constants::COLUMN_USERNAME_MAX_LENGTH);
        std::strncpy(statement.row_to_insert.email, email.c_str(), Constants::COLUMN_EMAIL_MAX_LENGTH);
        statement.row_to_insert.id = id;
        statement.row_to_insert.username[username.size()] = '\0';
        statement.row_to_insert.email[email.size()] = '\0';
        return PrepareResult::PREPARE_SUCCESS;
    } else if (token == "select") {
        if (iss >> token) { // exam if there are more tokens
            return PrepareResult::PREPARE_SYNTAX_ERROR;
        }
        statement.type = StatementType::STATEMENT_SELECT;
        return PrepareResult::PREPARE_SUCCESS;
    } else {
        return PrepareResult::PREPARE_UNRECOGNIZED;
    }
}


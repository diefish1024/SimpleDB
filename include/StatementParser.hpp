#ifndef STATEMENT_PARSER_HPP
#define STATEMENT_PARSER_HPP

#include "Statement.hpp"
#include <string>

PrepareResult prepareStatement(const std::string& input, Statement& statement);

#endif // STATEMENT_PARSER_HPP

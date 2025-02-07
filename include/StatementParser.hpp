#ifndef STATEMENT_PARSER_HPP
#define STATEMENT_PARSER_HPP

#include <string>
#include "Statement.hpp"

PrepareResult prepareStatement(const std::string& input, Statement& statement);

#endif // STATEMENT_PARSER_HPP

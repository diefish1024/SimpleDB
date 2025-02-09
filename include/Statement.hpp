#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "Row.hpp"

// SQL statement types
enum class StatementType {
    INSERT,
    SELECT
};

// SQL statement results
enum class PrepareResult {
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_NEGATIVE_ID,
    REPARE_STRING_TOO_LONG,
    PREPARE_UNRECOGNIZED
};

class Statement {
public:
    StatementType type;
    Row row_to_insert;
};

#endif // STATEMENT_HPP

#ifndef STATEMENT_HPP
#define STATEMENT_HPP

// SQL statement types
enum class StatementType {
    INSERT,
    SELECT
};

// SQL statement results
enum class PrepareResult {
    PREPARE_SUCCESS,
    PREPARE_FAILURE,
    PREPARE_UNRECOGNIZED
};

// Meta commands
enum class MetaCommandResult {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED
};

class Statement {
public:
    StatementType type;
};

#endif // STATEMENT_HPP

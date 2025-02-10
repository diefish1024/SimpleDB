#ifndef METACOMMAND_HPP
#define METACOMMAND_HPP

#include <string>

// Meta commands
enum class MetaCommandResult {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED,
    META_COMMAND_EXIT
};

MetaCommandResult doMetaCommand(const std::string& input);

#endif // METACOMMAND_HPP

#include "MetaCommand.hpp"
#include <iostream>

MetaCommandResult doMetaCommand(const std::string& input) {
    if (input == ".exit") {
        return MetaCommandResult::META_COMMAND_EXIT;
    }
    return MetaCommandResult::META_COMMAND_UNRECOGNIZED;
}

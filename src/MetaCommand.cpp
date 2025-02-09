#include "MetaCommand.hpp"
#include <iostream>

MetaCommandResult doMetaCommand(const std::string& input) {
    if (input == ".exit") {
        std::exit(0);
    }
    return MetaCommandResult::META_COMMAND_UNRECOGNIZED;
}

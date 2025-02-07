#include "MetaCommand.hpp"
#include <iostream>

MetaCommandResult doMetaCommand(const std::string& input) {
    if (input == ".exit") {
        std::exit(0);
    }
    std::cerr << "Unrecognized meta command: " << input << std::endl;
    return MetaCommandResult::META_COMMAND_UNRECOGNIZED;
}

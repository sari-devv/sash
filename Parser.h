#pragma once

#include <string>
#include <vector>
#include "Command.h"

class Parser {
public:
    std::vector<Command> parse(const std::string& input);
    static std::vector<std::string> tokenize(const std::string& input, char delimiter);
};

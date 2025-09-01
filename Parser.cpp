#include "Parser.h"
#include <sstream>

std::vector<Command> Parser::parse(const std::string& input) {
    std::vector<Command> commands;
    auto command_strings = tokenize(input, '|');

    for (const auto& command_str : command_strings) {
        std::stringstream command_ss(command_str);
        std::string token;
        Command cmd;
        bool first_token = true;

        while (command_ss >> token) {
            if (first_token) {
                cmd.setExecutable(token);
                first_token = false;
            } else {
                cmd.addArgument(token);
            }
        }
        commands.push_back(cmd);
    }

    return commands;
}

std::vector<std::string> Parser::tokenize(const std::string& input,
                                          char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

#include "Parser.h"
#include <sstream>

std::vector<Command> Parser::parse(const std::string& input) {
    std::vector<Command> commands;
    std::stringstream ss(input);
    std::string command_str;

    while (std::getline(ss, command_str, '|')) {
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

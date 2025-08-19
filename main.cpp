#include <iostream>
#include "Parser.h"

int main() {
    Parser parser;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line == "exit") {
            break;
        }

        auto commands = parser.parse(line);

        for (const auto& cmd : commands) {
            std::cout << "Executable: " << cmd.getExecutable() << std::endl;
            std::cout << "Arguments: ";
            for (const auto& arg : cmd.getArguments()) {
                std::cout << arg << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}

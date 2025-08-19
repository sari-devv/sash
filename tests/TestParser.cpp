#include <cassert>
#include <iostream>
#include "../Parser.h"

void testSimpleCommand() {
    Parser p;
    auto commands = p.parse("ls");
    assert(commands.size() == 1);
    assert(commands[0].getExecutable() == "ls");
    assert(commands[0].getArguments().empty());
    std::cout << "testSimpleCommand passed" << std::endl;
}

void testCommandWithArguments() {
    Parser p;
    auto commands = p.parse("ls -l -a");
    assert(commands.size() == 1);
    assert(commands[0].getExecutable() == "ls");
    assert(commands[0].getArguments().size() == 2);
    assert(commands[0].getArguments()[0] == "-l");
    assert(commands[0].getArguments()[1] == "-a");
    std::cout << "testCommandWithArguments passed" << std::endl;
}

void testPipedCommands() {
    Parser p;
    auto commands = p.parse("ls -l | grep .cpp");
    assert(commands.size() == 2);
    assert(commands[0].getExecutable() == "ls");
    assert(commands[0].getArguments().size() == 1);
    assert(commands[0].getArguments()[0] == "-l");
    assert(commands[1].getExecutable() == "grep");
    assert(commands[1].getArguments().size() == 1);
    assert(commands[1].getArguments()[0] == ".cpp");
    std::cout << "testPipedCommands passed" << std::endl;
}

void testMultiplePipedCommands() {
    Parser p;
    auto commands = p.parse("cat file.txt | grep hello | wc -l");
    assert(commands.size() == 3);
    assert(commands[0].getExecutable() == "cat");
    assert(commands[0].getArguments().size() == 1);
    assert(commands[0].getArguments()[0] == "file.txt");
    assert(commands[1].getExecutable() == "grep");
    assert(commands[1].getArguments().size() == 1);
    assert(commands[1].getArguments()[0] == "hello");
    assert(commands[2].getExecutable() == "wc");
    assert(commands[2].getArguments().size() == 1);
    assert(commands[2].getArguments()[0] == "-l");
    std::cout << "testMultiplePipedCommands passed" << std::endl;
}

void testEmptyCommand() {
    Parser p;
    auto commands = p.parse("");
    assert(commands.empty());
    std::cout << "testEmptyCommand passed" << std::endl;
}

int main() {
    testSimpleCommand();
    testCommandWithArguments();
    testPipedCommands();
    testMultiplePipedCommands();
    testEmptyCommand();
    return 0;
}
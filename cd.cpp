#include "cd.h"
#include "Command.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

using std::cerr;
using std::endl;

/* This function is now private to the cd command. */
static const char* getHomeDir() {
    const char* pHomeDirStr = std::getenv("HOME");
    assert(pHomeDirStr != nullptr);
    return pHomeDirStr;
}

int Cd::run(const Command& cmd) {
    assert(cmd.getExecutable() == "cd");

    auto arguments = cmd.getArguments();
    if (arguments.size() > 1) {
        cerr << "Too many arguments" << endl;
        return -1;
    }

    const char* targetDir;
    if (arguments.size() == 0) {
        targetDir = getHomeDir();
    } else {
        targetDir = arguments[0].c_str();
    }

    int res = chdir(targetDir);
    if (res != 0) {
        perror("Cannot change directory");
        return res;
    }

    return 0;
}

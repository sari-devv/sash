#include "Shell.h"
#include "Parser.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

using std::cerr;
using std::cout;
using std::endl;

/**
 * A default constructor. It initializes the home directory.
 */
Shell::Shell() {
}

/**
 * Initialize the shell.
 *
 * @return 0 on success, -1 on failure.
 */
int Shell::init() {
    const char* pHomeDir = getHomeDir();

    m_cwd = pHomeDir;
    int res = chdir(pHomeDir);
    if (res < 0) {
        perror("Can't set directory to HOME");
        return -1;
    }

    return res;
}

/**
 * @return A string containing the path of the home directory.
 */
const char* Shell::getHomeDir() {
    const char* pHomeDirStr = std::getenv("HOME");
    assert(pHomeDirStr != nullptr);

    return pHomeDirStr;
}

int Shell::run() {
    Parser parser;
    std::string line;

    /* The main logic of the shell, keep parsing commands until we get exit and
     * run each command. */
    while (true) {
        std::cout << m_prompt;
        if (!std::getline(std::cin, line)) {
            break;
        }

        if (line == "exit") {
            break;
        }

        auto commands = parser.parse(line);

        for (const auto& cmd : commands) {
            execute(cmd);
        }
    }

    return 0;
}

int Shell::execute(const Command& cmd) {
    /* Check if the command is a built in command, if it's not, execute it from
     * /bin. */
    auto it = m_builtinMap.find(cmd.getExecutable());
    if (it != m_builtinMap.end()) {
        return it->second(cmd);
    }

    else {
        cout << "Execute external cmd" << endl;
    }

    return 0;
}

/**************************** Builtins *******************************/

int Shell::cd(const Command& cmd) {
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

    /* Now set the cwd based on whether it was a relative path or not. */
    if (targetDir[0] == '/') {
        m_cwd = targetDir;
    } else {
        m_cwd += "/" + string(targetDir);
    }

    return 0;
}

int Shell::pwd(const Command& cmd) {
    cout << m_cwd << endl;

    return 0;
}

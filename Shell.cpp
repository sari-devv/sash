#include "Shell.h"
#include "Command.h"
#include "Parser.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits.h>
#include <sys/wait.h>
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
        executeExternalCmd(cmd);
    }

    return 0;
}

int Shell::executeExternalCmd(const Command& cmd) {
    /* We now need to execute an external command. We start with
     * assuming each executable is in /bin.
     * For each external cmd, we start a new process and run it using execve().
     */
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork() failed");
        return 1;
    }

    if (pid == 0) {
        /* Child process code, run execve(). */
        string executable = cmd.getExecutable();
        if (executable[0] != '/') {
            /* We were given a relative path. */
            executable = "/bin/" + executable;
        }
        execv(executable.c_str(),
              const_cast<char* const*>(cmd.getArgsAsCharVec().data()));
        perror("Execv failed");
        exit(EXIT_FAILURE);
    } else {
        /* Parent process, wait for the child. */
        int status;
        int res = waitpid(pid, &status, 0);
        if (res < 0) {
            perror("waitpid failed");
            return 1;
        }
    }

    return 0;
}

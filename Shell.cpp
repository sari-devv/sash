#include "Shell.h"
#include "Command.h"
#include "Parser.h"

#include <cassert>
#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <limits.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

namespace {

/**
 * An RAII wrapper for managing the terminal in raw mode.
 * The constructor enables raw mode, and the destructor restores the original mode.
 */
class RawModeTerminal {
public:
    RawModeTerminal() {
        // Get the original terminal attributes
        tcgetattr(STDIN_FILENO, &m_orig_termios);

        // Create a raw copy and disable canonical mode and echo
        termios raw = m_orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON);

        // Apply the raw attributes
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    }

    ~RawModeTerminal() {
        // Restore the original terminal attributes
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &m_orig_termios);
    }

    // Disable copy and assignment
    RawModeTerminal(const RawModeTerminal&) = delete;
    RawModeTerminal& operator=(const RawModeTerminal&) = delete;

private:
    termios m_orig_termios;
};

std::string getLineRaw(const std::string& prompt) {
    // The RawModeTerminal object handles setup and teardown automatically.
    RawModeTerminal raw_mode;

    std::string line;
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (iscntrl(c)) {
            if (c == 12) { // CTRL-L
                // Clear screen, then redraw prompt and current line
                write(STDOUT_FILENO, "\033[H\033[2J", 7);
                write(STDOUT_FILENO, prompt.c_str(), prompt.length());
                write(STDOUT_FILENO, line.c_str(), line.length());
            } else if (c == '\n' || c == '\r') {
                write(STDOUT_FILENO, "\n", 1);
                break;
            } else if (c == 127) { // Backspace
                if (!line.empty()) {
                    line.pop_back();
                    // Move cursor back, write space, move back again
                    write(STDOUT_FILENO, "\b \b", 3);
                }
            }
        } else {
            line += c;
            write(STDOUT_FILENO, &c, 1);
        }
    }
    return line;
}
} // namespace

/**
 * A default constructor. It initializes the home directory.
 */
Shell::Shell() {
    m_prompt = "[sash " + std::to_string(getpid()) + "]$ ";
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

    const char* pPath = std::getenv("PATH");
    if (pPath != nullptr) {
        m_path = Parser::tokenize(pPath, ':');
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
        std::cout << m_prompt << std::flush;
        line = getLineRaw(m_prompt);

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
            /* We were given a relative path, look for the executable in each
             * directory of PATH. */
            executable = findExecutableInPath(executable);
            if (executable.empty()) {
                exit(1);
            }
        }
        auto args = cmd.getArgsAsCharVec();
        execv(executable.c_str(), const_cast<char* const*>(args.data()));
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

string Shell::findExecutableInPath(string executable) {
    for (const string& dir : m_path) {
        string path = dir + "/" + executable;
        int res = access(path.c_str(), X_OK);
        if (res == 0) {
            return path;
        }
        /* access(2) failed. Check the reason. */
        if (errno == ENOENT) {
            /* If the file does not exist in this directory, check the next
             * directory in PATH. */
            continue;
        }

        /* Else, print the error and exit. */
        perror("sash");
        return "";
    }

    /* If errno wasn't ENOENT, print it. Else print a custom error message. */
    if (errno != ENOENT) {
        perror("sash");
    } else {
        cerr << "sash: command not found" << endl;
    }

    return "";
}
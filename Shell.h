#pragma once

#include "Command.h"
#include "cd.h"
#include "pwd.h"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::unordered_map;
using std::vector;

class Shell {
  public:
    Shell();
    int init();
    int run();

  private:
    const char* getHomeDir();
    int execute(const Command& cmd);
    int executeExternalCmd(const Command& cmd);
    string findExecutableInPath(string executable);

    string m_prompt{"$ "};

    vector<string> m_path;
    vector<string> m_history;

    /* Builtins. */
    using Builtin = std::function<int(const Command& cmd)>;

    unordered_map<string, Builtin> m_builtinMap;
};

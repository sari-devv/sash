#pragma once

#include "Command.h"
#include "cd.h"
#include "pwd.h"
#include <functional>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

class Shell {
  public:
    Shell();
    int init();
    int run();

  private:
    const char* getHomeDir();
    int execute(const Command& cmd);
    int executeExternalCmd(const Command& cmd);

    string m_prompt{"$ "};

    /* Builtins. */
    using Builtin = std::function<int(const Command& cmd)>;

    unordered_map<string, Builtin> m_builtinMap{{"cd", &Cd::run},
                                                {"pwd", &Pwd::run}};
};

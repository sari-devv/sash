#pragma once

#include "Command.h"
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

    string m_cwd;
    string m_prompt{"$ "};

    /* Builtins. */
    using Builtin = std::function<int(const Command& cmd)>;

    int cd(const Command& cmd);
    int pwd(const Command& cmd);

    unordered_map<string, Builtin> m_builtinMap{
        {"cd", [this](const Command& cmd) { return this->cd(cmd); }},
        {"pwd", [this](const Command& cmd) { return this->pwd(cmd); }}};
};

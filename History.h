#pragma once

#include "Command.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class History {
public:
    History(const vector<string>& history);
    int run(const Command& cmd) const;

private:
    const vector<string>& m_history;
};
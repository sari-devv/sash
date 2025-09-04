#include "History.h"
#include <iostream>

using std::cout;
using std::endl;

History::History(const vector<string>& history) : m_history(history) {}

int History::run(const Command& cmd) const {
    for (size_t i = 0; i < m_history.size(); ++i) {
        cout << i + 1 << "  " << m_history[i] << endl;
    }
    return 0;
}
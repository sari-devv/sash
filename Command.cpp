#include "Command.h"

void Command::setExecutable(const std::string& executable) {
    m_executable = executable;
}

void Command::addArgument(const std::string& argument) {
    m_arguments.push_back(argument);
}

const std::string& Command::getExecutable() const {
    return m_executable;
}

const vector<std::string>& Command::getArguments() const {
    return m_arguments;
}

vector<const char*> Command::getArgsAsCharVec() const {
    vector<const char*> strVec;
    strVec.push_back(m_executable.c_str());
    for (const auto& arg : m_arguments) {
        strVec.push_back(arg.c_str());
    }
    strVec.push_back(nullptr);
    return strVec;
}

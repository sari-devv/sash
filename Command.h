#pragma once

#include <string>
#include <vector>

class Command {
public:
    void setExecutable(const std::string& executable) {
        m_executable = executable;
    }

    void addArgument(const std::string& argument) {
        m_arguments.push_back(argument);
    }

    const std::string& getExecutable() const {
        return m_executable;
    }

    const std::vector<std::string>& getArguments() const {
        return m_arguments;
    }

private:
    std::string m_executable;
    std::vector<std::string> m_arguments;
};
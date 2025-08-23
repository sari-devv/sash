#pragma once

#include <string>
#include <vector>

using std::vector;

class Command {
  public:
    void setExecutable(const std::string& executable);
    void addArgument(const std::string& argument);
    const std::string& getExecutable() const;
    const vector<std::string>& getArguments() const;
    vector<const char*> getArgsAsCharVec() const;

  private:
    std::string m_executable;
    std::vector<std::string> m_arguments;
};

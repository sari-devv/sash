#include "pwd.h"
#include "Command.h"
#include <iostream>
#include <limits.h>
#include <unistd.h>

using std::cout;
using std::endl;

int Pwd::run(const Command& cmd) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        cout << cwd << endl;
    } else {
        perror("getcwd() error");
        return 1;
    }
    return 0;
}

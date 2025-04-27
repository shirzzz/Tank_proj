#ifndef SHELLS_H
#define SHELLS_H
#include <algorithm>
#include <vector>
#include <iostream>
#include "Shell.h"

static_assert(sizeof(Shell) > 0, "Shell is not being included!");
class Shells {
private:
    std::vector<Shell> shellsList;

public:
    Shells() = default;

    // Non-const access for modification
    std::vector<Shell>& accessShells() {
        return shellsList;
    }

    // Const access for read-only use
    const std::vector<Shell>& getShellsList() const {
        if (shellsList.empty()) {
            std::cout << "Shells list is null!" << std::endl;
        }
        std::cout << "Shells size: " << shellsList.size() << std::endl;
        return shellsList;
    }

    // Add a new shell to the list
    void addShelltoList(const Shell& shell) {
        std::cout << "Adding shell at (" << shell.getX() << ", " << shell.getY() << ")" << std::endl;
        if(shellsList.empty()) {
            std::cout << "Shells list is null!" << std::endl;
            return;
        }
        shellsList.push_back(shell);
        std::cout << "Shells size: " << shellsList.size() << std::endl;
        std::cout << "Shell added." << std::endl;

    }

    // Remove shell at specific coordinates
    bool removeShellAt(int x, int y) {
        auto it = std::remove_if(shellsList.begin(), shellsList.end(), [&](const Shell& s) {
            return s.getX() == x && s.getY() == y;
        });
        if (it != shellsList.end()) {
            shellsList.erase(it, shellsList.end());
            return true;
        }
        return false;
    }
};

#endif // SHELLS_H

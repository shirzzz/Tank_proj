#ifndef SHELLS_H
#define SHELLS_H

#include <vector>

class Shells {
    private:
        std::vector<Shell> shellsList;
    
    public:
        Shells() = default;
    
        std::vector<Shell>& accessShells() {
            return shellsList;
        }
    
        void addShell(const Shell& shell) {
            shellsList.push_back(shell);
        }
    
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
    
        std::vector<Shell>& accessShells() {
            return shellsList;
        }
    };

#endif // SHELLS_H
using namespace std;
#include <iostream>
#include <vector>
//directions: 0 = right, 1 = left, 2 = up, 3 = left
vector<vector<char>> grid;
class Actions {
    void move_forward(int x, int y, int direction, char index_tank) {
        grid[x][y] = ' '; // Clear the current position
        if(direction == 0) {
            grid[x+1][y] = index_tank; // Move right
        } else if(direction == 1) {
                grid[x-1][y] = index_tank; // Move left
            }
            else if(direction == 2) {
                grid[x][y+1] = index_tank; // Move up
            } else if(direction == 3) {
                grid[x][y-1] = index_tank; // Move down
            }
    }
    void move_backward(int x, int y, int direction, char index_tank) {
        grid[x][y] = ' '; // Clear the current position
        if(direction == 0) {
            grid[x-1][y] = index_tank; // Move left
        } else if(direction == 1) {
                grid[x+1][y] = index_tank; // Move right
            }
            else if(direction == 2) {
                grid[x][y-1] = index_tank; // Move down
            } else if(direction == 3) {
                grid[x][y+1] = index_tank; // Move up
            }
    }
    void shoot() {

    }

};

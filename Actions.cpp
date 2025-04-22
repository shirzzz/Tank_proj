#include <iostream>
#include <vector>
using namespace std;

// Global game grid representation
vector<vector<char>> grid;

// Directions: 0 = right, 1 = left, 2 = up, 3 = down
class Actions {
public:
    // Move tank forward based on its direction
    void move_forward(int x, int y, int direction, char index_tank) {
        grid[x][y] = ' '; // Clear the current position
        switch (direction) {
            case 0: grid[x + 1][y] = index_tank; break; // Right
            case 1: grid[x - 1][y] = index_tank; break; // Left
            case 2: grid[x][y + 1] = index_tank; break; // Up
            case 3: grid[x][y - 1] = index_tank; break; // Down
        }
    }

    // Move tank backward based on its direction
    void move_backward(int x, int y, int direction, char index_tank) {
        grid[x][y] = ' '; // Clear the current position
        switch (direction) {
            case 0: grid[x - 1][y] = index_tank; break; // Left
            case 1: grid[x + 1][y] = index_tank; break; // Right
            case 2: grid[x][y - 1] = index_tank; break; // Down
            case 3: grid[x][y + 1] = index_tank; break; // Up
        }
    }

    // Placeholder for shooting logic
    void shoot() {
        // Implement shooting logic here
    }
};
#include "BFSChaserAI.h"
#include "CanonDirection.h"
#include "CellType.h"
#include "GameBoard.h"
#include "Tank.h"
#include "ActionType.h"
#include "Shell.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "DirectionUtils.h"
#include <cmath>


// Checks whether the tile directly in front of the tank is in the path of any shell
// Simulates shell movement and returns true if the shell will collide with the forward tile
bool BFSChaserAI::isDangerAhead(const Tank& tank, const GameBoard& board) {
    int tx = tank.getX();
    int ty = tank.getY();
    auto dir = directionToVector(tank.getDirection());
    int dx = dir.first;
    int dy = dir.second;

    int fx = tx + dx;
    int fy = ty + dy;

    // Check each shell to see if it will pass through (fx, fy)
    for (const Shell& shell : board.getShells()) {
        int sx = shell.getX();
        int sy = shell.getY();
        auto sdir = directionToVector(shell.getDirection());
        int sdx = sdir.first;
        int sdy = sdir.second;
        

        // Simulate 5 future steps of shell movement
        for (int step = 0; step < 5; ++step) {
            if (sx == fx && sy == fy) return true;
            sx += sdx;
            sy += sdy;

            if (sx < 0 || sx >= board.getWidth() || sy < 0 || sy >= board.getHeight()) break;
        }
    }

    return false;
}

// Main decision function of the BFSChaserAI tank
// Chooses an action each turn: move, rotate, or shoot
ActionType BFSChaserAI::decideNextAction(const GameBoard& board, const Tank& self, const Tank& opponent) {
    // Step 1: Try to get a BFS path toward the opponent
    auto target = findNextStepTowardOpponent(board, self, opponent);
    int targetX = target.first;
    int targetY = target.second;


    
    // Step 2: If no path is found, fallback to rotating and shooting
    if (targetX == -1 || targetY == -1) {
        int dx = opponent.getX() - self.getX();
        int dy = opponent.getY() - self.getY();
        CanonDirection desiredDir = getDirectionFromDelta(dx, dy);
        CanonDirection currentDir = self.getDirection();

        if (currentDir != desiredDir) {
            int curIdx = static_cast<int>(currentDir);
            int desIdx = static_cast<int>(desiredDir);
            int diff = (desIdx - curIdx + 8) % 8;

            // Choose best rotation
            if (diff == 1 || diff == 2) return ActionType::ROTATE_EIGHTH_RIGHT;
            if (diff == 6 || diff == 7) return ActionType::ROTATE_EIGHTH_LEFT;
            if (diff == 3) return ActionType::ROTATE_QUARTER_RIGHT;
            if (diff == 5) return ActionType::ROTATE_QUARTER_LEFT;
            if (diff == 4) return ActionType::ROTATE_QUARTER_RIGHT;
        }

        return ActionType::SHOOT; // Already aligned
    }

    // Step 3: Path found - align with direction and move forward
    int dx = targetX - self.getX();
    int dy = targetY - self.getY();
    CanonDirection desiredDir = getDirectionFromDelta(dx, dy);
    CanonDirection currentDir = self.getDirection();

    // Step 4: Already aligned - move unless there's danger ahead
    if (currentDir == desiredDir) {
        if (!isDangerAhead(self, board)) return ActionType::MOVE_FORWARD;
        return ActionType::ROTATE_EIGHTH_LEFT; // dodge
    }

    // Step 5: Not aligned - choose best rotation toward direction
    int curIdx = static_cast<int>(currentDir);
    int desIdx = static_cast<int>(desiredDir);
    int diff = (desIdx - curIdx + 8) % 8;

    if (diff == 1 || diff == 2) return ActionType::ROTATE_EIGHTH_RIGHT;
    if (diff == 6 || diff == 7) return ActionType::ROTATE_EIGHTH_LEFT;
    if (diff == 3) return ActionType::ROTATE_QUARTER_RIGHT;
    if (diff == 5) return ActionType::ROTATE_QUARTER_LEFT;
    if (diff == 4) return ActionType::ROTATE_QUARTER_RIGHT;

    return ActionType::MOVE_FORWARD;
}

// Breadth-First Search to find the shortest walkable path to the opponent
// Skips mines and walls
std::pair<int, int> BFSChaserAI::findNextStepTowardOpponent(
    const GameBoard& board,
    const Tank& self,
    const Tank& opponent
) {
    int width = board.getWidth();
    int height = board.getHeight();

    using Coord = std::pair<int, int>;
    std::queue<Coord> q;
    std::unordered_map<int, Coord> cameFrom;
    std::unordered_set<int> visited;

    auto index = [&](int x, int y) {
        return y * width + x;
    };

    int startX = self.getX();
    int startY = self.getY();
    int goalX = opponent.getX();
    int goalY = opponent.getY();

    q.push({startX, startY});
    visited.insert(index(startX, startY));

    std::vector<Coord> directions = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };

    while (!q.empty()) {
        auto front = q.front();
        int x = front.first;
        int y = front.second;
        q.pop();

        if (x == goalX && y == goalY) {
            // Backtrack to find first step in the path
            while (cameFrom.count(index(x, y)) && cameFrom[index(x, y)] != Coord{startX, startY}) {
                std::tie(x, y) = cameFrom[index(x, y)];
            }
            return {x, y};
        }

        for (const auto& dir : directions) {
            int dx = dir.first;
            int dy = dir.second;
        
            int nx = x + dx;
            int ny = y + dy;
        
            if (nx < 0 || ny < 0 || nx >= width || ny >= height) continue;
            if (visited.count(index(nx, ny))) continue;
        
            CellType cell = board.getCell(nx, ny);
            if (cell == CellType::WALL || cell == CellType::MINE) continue;
        
            visited.insert(index(nx, ny));
            cameFrom[index(nx, ny)] = std::make_pair(x, y);  // use make_pair for GCC 6.3
            q.push(std::make_pair(nx, ny));                  // same here
        }
    }

    return {-1, -1}; // No path found
}

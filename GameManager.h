#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#pragma once

#include "GameBoard.h"
#include "ActionType.h"
#include "Tank.h"
#include <string>
#include <climits>  // For INT_MAX

class GameManager {
private:
    GameBoard& game_board;
    bool game_over = false;

    Tank* tank1 = nullptr;
    Tank* tank2 = nullptr;
    int moves_left = INT_MAX;

    // Stores last known tanks for final action logging or destruction cause
    Tank* lastKnownTank1 = nullptr;
    Tank* lastKnownTank2 = nullptr;

public:
    GameManager(GameBoard& board) : game_board(board) {
        tank1 = board.getTank1();
        tank2 = board.getTank2();
    }

    // Core game progression
    void updateGame();             // Advance tank logic
    void updateShells();           // Move all shells

    // Collision handlers
    void resolveShellCollisions(); // Handle shell collisions with tanks/walls/other shells
    void resolveTankCollisions();  // Handle tank collisions with tanks/mines

    // Game status
    void displayGame();            // Display actions and outcomes
    void endGame();                // Ends the game

    // Action processor
    void processAction(Tank* tank, ActionType action, const std::string& name);

    // Accessors
    bool isGameOver() const { return game_over; }
    Tank* getTank1() { return tank1; }
    Tank* getTank2() { return tank2; }
    int getMovesLeft() const { return moves_left; }

    // Modifiers
    void setMovesLeft(int moves) { moves_left = moves; }
    void removeTank(char index);   // Remove a tank from the game board
};

#endif // GAMEMANAGER_H

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#pragma once

#include "ActionType.h"
#include "Tank.h"
#include <string>
#include <climits>  // For INT_MAX
#include <memory>
#include "GameBoard.h"

class GameManager {
private:
    std::shared_ptr<GameBoard> shared_board; // Shared pointer to the game board
    bool game_over = false;

    Player1 player1;
    Player2 player2;
    char wining_tank = '0'; // 0 for draw, 1 for tank1, 2 for tank2
    int moves_left = INT_MAX;

    // Stores last known tanks for final action logging or destruction cause
    std::shared_ptr<Tank> lastKnownTank1 = nullptr;
    std::shared_ptr<Tank> lastKnownTank2 = nullptr;

public:
        GameManager(std::shared_ptr<GameBoard> shared_board) : shared_board(shared_board) {
        tank1 = shared_board->getTank1();
        tank2 = shared_board->getTank2();
    }
    // ~GameManager() = default;

    // Core game progression
    void updateGame();             // Advance tank logic
    void updateShells() const;           // Move all shells

    // Collision handlers
    void resolveShellCollisions(); // Handle shell collisions with tanks/walls/other shells
    void resolveTankCollisions();  // Handle tank collisions with tanks/mines

    // Game status
    void displayGame() const;            // Display actions and outcomes
    void endGame();                // Ends the game

    // Action processor
    void processAction(std::shared_ptr<Tank> tank, ActionType action, const std::string& name);

    // Accessors
    bool isGameOver() const { return game_over; }
    std::shared_ptr<Tank> getTank1() { return tank1; }
    std::shared_ptr<Tank> getTank2() { return tank2; }
    int getMovesLeft() const { return moves_left; }

    // Modifiers
    void setMovesLeft(int moves) { moves_left = moves; }
    void removeTank(char index);   // Remove a tank from the game board
    void setGameOver(bool game_over) { this->game_over = game_over; }
    bool getGameOver() const { return game_over; }
    std::vector<Shell>& getmyShells() { return shared_board->getShells(); }
    std::shared_ptr<GameBoard> getBoard() { return shared_board; }
    void readBoard(std::istream& file_board, std::string filename);
    bool isGameEnded() const ();
    void run();
};

#endif // GAMEMANAGER_H

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#pragma once

#include "common/ActionRequest.h"
#include "Tank.h"
#include "common/PlayerFactory.h"
#include "common/TankAlgorithmFactory.h"
#include "common/Player1.h"
#include "common/Player2.h"
#include <string>
#include <climits>  // For INT_MAX
#include <memory>
#include "GameBoard.h"
#include <fstream>

class GameManager {
private:
    std::shared_ptr<GameBoard> shared_board; // Shared pointer to the game board
    bool game_over = false;

    // Assignment 2 - Factory references
    PlayerFactory* playerFactory = nullptr;
    TankAlgorithmFactory* tankFactory = nullptr;

    Player1 player1;
    Player2 player2;
    char wining_tank = '0'; // 0 for draw, 1 for tank1, 2 for tank2
    int moves_left = INT_MAX;

    // Stores last known tanks for final action logging or destruction cause
    std::shared_ptr<Tank> lastKnownTank1 = nullptr;
    std::shared_ptr<Tank> lastKnownTank2 = nullptr;
    std::shared_ptr<Tank> tank1 = nullptr;
    std::shared_ptr<Tank> tank2 = nullptr;

public:
    // Assignment 2 Constructor - Takes factories as required by assignment
    GameManager(PlayerFactory& pf, TankAlgorithmFactory& tf) 
        : playerFactory(&pf), tankFactory(&tf) {
        // Create default board - will be updated when reading from file
        shared_board = std::make_shared<GameBoard>(10, 8);
    }

    // Assignment 1 Constructor - Keep for backward compatibility
    GameManager(std::shared_ptr<GameBoard> shared_board) : shared_board(shared_board) {
        if (shared_board) {
            tank1 = shared_board->getTank1(); 
            tank2 = shared_board->getTank2();
        }
    }

    // ~GameManager() = default;

    // Core game progression
    void updateGame();             // Advance tank logic
    void updateShells() const;     // Move all shells

    // Collision handlers
    void resolveShellCollisions(); // Handle shell collisions with tanks/walls/other shells
    void resolveTankCollisions();  // Handle tank collisions with tanks/mines

    // Game status
    void displayGame() const;      // Display actions and outcomes
    void endGame();                // Ends the game

    // Action processor
    void processAction(std::shared_ptr<Tank> tank, ActionRequest action, const std::string& name);

    // Accessors
    bool isGameOver() const { return game_over; }
    std::shared_ptr<Tank> getTank1() { return tank1; }
    std::shared_ptr<Tank> getTank2() { return tank2; }
    int getMovesLeft() const { return moves_left; }

    // Assignment 2 - Factory accessors
    PlayerFactory* getPlayerFactory() const { return playerFactory; }
    TankAlgorithmFactory* getTankAlgorithmFactory() const { return tankFactory; }

    // Modifiers
    void setMovesLeft(int moves) { moves_left = moves; }
    void removeTank(char index);   // Remove a tank from the game board
    void setGameOver(bool game_over) { this->game_over = game_over; }
    bool getGameOver() const { return game_over; }
    std::vector<Shell>& getmyShells() { return shared_board->getShells(); }
    std::shared_ptr<GameBoard> getBoard() { return shared_board; }
    
    // File reading methods
    void readBoard(std::istream& file_board, std::string filename);
    
    // Assignment 2 - Direct filename reading
    void readBoard(const std::string& filename) {
        std::ifstream file_board(filename);
        if (!file_board.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }
        readBoard(file_board, filename);
        file_board.close();
    }
    
    bool isGameEnded() const;
    void run();
    
    // Assignment 2 - Win condition checker
    void checkWinCondition();
};

#endif // GAMEMANAGER_H
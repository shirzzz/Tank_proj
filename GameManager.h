#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#pragma once

#include "common/ActionRequest.h"
#include "Tank.h"
#include "MyPlayerFactory.h"  // CHANGED: Use your concrete factory
#include "MyTankAlgorithmFactory.h"  // CHANGED: Use your concrete factory
#include "Player1.h"
#include "Player2.h"
#include <string>
#include <climits>  // For INT_MAX
#include <memory>
#include "GameBoard.h"
#include <fstream>

class GameManager {
private:
    std::shared_ptr<GameBoard> shared_board; // Shared pointer to the game board
    bool game_over = false;
    std::vector<std::vector<bool>> is_mine;
    // Assignment 2 - Factory references
    MyPlayerFactory* player_factory = nullptr;  // CHANGED: Use concrete type
    MyTankAlgorithmFactory* tank_factory = nullptr;  // CHANGED: Use concrete type
    size_t max_steps = 0; // Maximum steps allowed in the game
    size_t num_shells = 0; // Number of shells available for each tank
    size_t width = 0; // Width of the game board
    size_t height = 0; // Height of the game board

    Player1 player1;
    Player2 player2;
    char wining_tank = '0'; // 0 for draw, 1 for tank1, 2 for tank2
    int moves_left = INT_MAX;

    //Class methods which are only used in this class
    
    // Core game progression
    void updateGame();             // Advance tank logic
    void updateShells() const;     // Move all shells

     // Collision handlers
    void resolveShellCollisions(); // Handle shell collisions with tanks/walls/other shells
    void resolveTankCollisions();  // Handle tank collisions with tanks/mines

    // Game status
    void displayGame() const;      // Display actions and outcomes
    void endGame();                // Ends the game
    
    void processAction(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm, ActionRequest action);

    // Accessors
    bool isGameOver() const { return game_over; }
    int getMovesLeft() const { return moves_left; }

    // Modifiers
    void setMovesLeft(int moves) { moves_left = moves; }
    void setGameOver(bool game_over) { this->game_over = game_over; }
    bool getGameOver() const { return game_over; }
    std::vector<Shell>& getmyShells() { return shared_board->getShells(); }
    std::shared_ptr<GameBoard> getBoard() { return shared_board; }
        
    bool isGameEnded();
        
    // Assignment 2 - Win condition checker
    void checkWinCondition();
    void updateTanks() const; // Update tanks' positions and actions

public:
    // Assignment 2 Constructor - Takes factories as required by assignment
    GameManager(MyPlayerFactory& pf, MyTankAlgorithmFactory& tf)  // CHANGED: Use concrete types
        : player_factory(&pf), tank_factory(&tf) {

    }

    // Assignment 2 - Factory accessors
    MyPlayerFactory* getPlayerFactory() const { return player_factory; }  // CHANGED: Return concrete type
    MyTankAlgorithmFactory* getTankAlgorithmFactory() const { return tank_factory; }  // CHANGED: Return concrete type
    
    // Assignment 2 - Direct filename reading
    void readBoard(const std::string& filename);
    void run();
    bool loadBoardFromFile(std::istream& file_board, std::string filename);

};

#endif // GAMEMANAGER_H
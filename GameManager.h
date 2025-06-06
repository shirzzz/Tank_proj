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
    PlayerFactory* player_factory = nullptr;
    TankAlgorithmFactory* tank_factory = nullptr;
    size_t max_steps = 0; // Maximum steps allowed in the game
    size_t num_shells = 0; // Number of shells available for each tank
    size_t width = 0; // Width of the game board
    size_t height = 0; // Height of the game board

    Player1 player1;
    Player2 player2;
    char wining_tank = '0'; // 0 for draw, 1 for tank1, 2 for tank2
    int moves_left = INT_MAX;

    // // Stores last known tanks for final action logging or destruction cause
    // std::shared_ptr<Tank> lastKnownTank1 = nullptr;
    // std::shared_ptr<Tank> lastKnownTank2 = nullptr;
    // std::shared_ptr<Tank> tank1 = nullptr;
    // std::shared_ptr<Tank> tank2 = nullptr;

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
    
    // Action processor
    void processAction(std::shared_ptr<Tank> tank, TankAlgorithm tank_algorithm, ActionRequest action);

    // Accessors
    bool isGameOver() const { return game_over; }
    // std::shared_ptr<Tank> getTank1() { return tank1; }
    // std::shared_ptr<Tank> getTank2() { return tank2; }
    int getMovesLeft() const { return moves_left; }

    // Modifiers
    void setMovesLeft(int moves) { moves_left = moves; }
    //Isnt it the GameBoard's responsibility to remove tanks?
    //void removeTank(char index);   // Remove a tank from the game board
    void setGameOver(bool game_over) { this->game_over = game_over; }
    bool getGameOver() const { return game_over; }
    std::vector<Shell>& getmyShells() { return shared_board->getShells(); }
    std::shared_ptr<GameBoard> getBoard() { return shared_board; }
        
    bool isGameEnded();
        
    // Assignment 2 - Win condition checker
    void checkWinCondition();

public:
    // Assignment 2 Constructor - Takes factories as required by assignment
    GameManager(PlayerFactory& pf, TankAlgorithmFactory& tf) 
        : player_factory(&pf), tank_factory(&tf) {

    }

    // // Assignment 1 Constructor - Keep for backward compatibility
    // GameManager(std::shared_ptr<GameBoard> shared_board) : shared_board(shared_board) {
    //     if (shared_board) {
    //         tank1 = shared_board->getTank1(); 
    //         tank2 = shared_board->getTank2();
    //     }
    // }

    // ~GameManager() = default;

    // Assignment 2 - Factory accessors
    PlayerFactory* getPlayerFactory() const { return player_factory; }
    TankAlgorithmFactory* getTankAlgorithmFactory() const { return tank_factory; }
    
    // Assignment 2 - Direct filename reading
    void readBoard(const std::string& filename);
    void run();
    bool loadBoardFromFile(std::istream& file_board, std::string filename);

};

#endif // GAMEMANAGER_H
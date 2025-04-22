#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#pragma once
#include "GameBoard.h"
#include "string"
#include "ActionType.h"

class GameManager {
private:
    GameBoard& game_board;
    bool game_over = false;
    std::vector<std::string> actions_tank1;
    std::vector<std::string> actions_tank2;

public:
    GameManager(GameBoard& board) : game_board(board) {};
    void updateGame();
    void updateShells();
    void displayGame();
    void endGame();
    void processAction(Tank* tank, ActionType action, const std::string& name);
    bool isGameOver() const { return game_over; }
};



#endif //GAMEMANAGER_H

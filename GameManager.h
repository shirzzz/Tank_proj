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
    Tank *tank1 = nullptr;
    Tank *tank2 = nullptr;

public:
    GameManager(GameBoard& board) : game_board(board) {
        tank1 = board.getTank1();
        tank2 = board.getTank2();
    };
    void updateGame();
    void updateShells();
    void displayGame();
    void endGame();
    void processAction(Tank* tank, ActionType action, const std::string& name);
    bool isGameOver() const { return game_over; }
};



#endif //GAMEMANAGER_H

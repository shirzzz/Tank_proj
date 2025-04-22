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
    int moves_left = INT_MAX;

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
    Tank* getTank1() { return tank1; }
    Tank* getTank2() { return tank2; }
    int getMovesLeft() const { return moves_left; }
    void setMovesLeft(int moves) { moves_left = moves; }
};



#endif //GAMEMANAGER_H

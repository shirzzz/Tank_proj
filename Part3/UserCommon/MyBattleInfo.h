// MyBattleInfo.h - Clean header with declaration only
#pragma once
#include "../common/BattleInfo.h"
#include "MySatelliteView.h"
#include "Tank.h"
#include "../Algorithm/GameBoard.h"
#include <vector>
#include <string>
#include <utility>
class MyBattleInfo : public BattleInfo {
public:
    // Tank-specific fields
    char myPlayerChar; // Character representing this player's tank
    std::vector<std::shared_ptr<Tank>> myTanks; // List of this player's tanks
    std::vector<std::pair<size_t, size_t>> knownEnemyLocations;
    Algorithm_211466123_212399455::GameBoard* gameBoard = nullptr; // Pointer to the game board

    // Constructor declaration
    MyBattleInfo(MySatelliteView* satelliteView, char myPlayerChar = '1', std::vector<std::shared_ptr<Tank>> myTanks = std::vector<std::shared_ptr<Tank>>());
    
    std::vector<std::pair<size_t, size_t>> getOpponents() const;
    Algorithm_211466123_212399455::GameBoard* getGameBoard() const;
};
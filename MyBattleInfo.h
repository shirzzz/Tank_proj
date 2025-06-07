// MyBattleInfo.h - Clean header with declaration only
#pragma once
#include "common/BattleInfo.h"
#include "MySatelliteView.h"
#include "Tank.h"
#include "GameBoard.h"
#include <vector>
#include <string>
#include <utility>

// FIXED: Moved class definition to proper location in header
class MyBattleInfo : public BattleInfo {
public:
    // Tank-specific fields
    char myPlayerChar; // Character representing this player's tank
    std::vector<std::shared_ptr<Tank>> myTanks; // List of this player's tanks
    std::vector<std::pair<size_t, size_t>> knownEnemyLocations;
    GameBoard* gameBoard = nullptr; // Pointer to the game board

    // Constructor declaration
    MyBattleInfo(MySatelliteView* satelliteView, char myPlayerChar = '1', std::vector<std::shared_ptr<Tank>> myTanks = std::vector<std::shared_ptr<Tank>>());
    
    // ADDED: Missing method declarations
    std::vector<std::pair<size_t, size_t>> getOpponents() const;
    GameBoard* getGameBoard() const;
}; // ADDED: Missing semicolon
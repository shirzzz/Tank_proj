// MyBattleInfo.h - Clean header with declaration only
#pragma once
#include "BattleInfo.h"
#include "MySatelliteView.h"
#include "Tank.h"
#include "GameBoard.h"
#include <vector>
#include <string>
#include <utility>

class MyBattleInfo : public BattleInfo {
public:
    // Tank-specific fields
    std::vector<std::pair<size_t, size_t>> knownEnemyLocations;
    GameBoard* gameBoard = nullptr; // Pointer to the game board

    // Constructor declaration only
    MyBattleInfo(MySatelliteView* satelliteView, char myPlayerChar = '1');
};

// MyBattleInfo.cpp - Implementation file
#include "MyBattleInfo.h"

MyBattleInfo::MyBattleInfo(MySatelliteView* satelliteView, char myPlayerChar) {
    if (!satelliteView) return;
    
    // 1. Get the board pointer from MySatelliteView
    gameBoard = const_cast<GameBoard*>(satelliteView->getBoardReference());
    
    // 2. Clear enemy locations before scanning
    knownEnemyLocations.clear();
    
    // 3. Determine enemy character based on my player character
    char enemyChar = (myPlayerChar == '1') ? '2' : '1';
    
    // 4. Get battlefield dimensions from the board directly
    size_t max_width = static_cast<size_t>(gameBoard->getWidth());
    size_t max_height = static_cast<size_t>(gameBoard->getHeight());
    
    // 5. Scan the entire battlefield for enemy tank locations
    for (size_t y = 0; y < max_height; ++y) {
        for (size_t x = 0; x < max_width; ++x) {
            char cellContent = satelliteView->getObjectAt(x, y);
            
            // Check if this cell contains an enemy tank
            if (cellContent == enemyChar) {
                knownEnemyLocations.emplace_back(x, y);
            }
        }
    }
}
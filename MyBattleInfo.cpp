// MyBattleInfo.cpp - Implementation file
#include "MyBattleInfo.h"

// REMOVED: Class definition (belongs in header file)
// REMOVED: Extra blank lines and misplaced comments

MyBattleInfo::MyBattleInfo(MySatelliteView* satelliteView, char myPlayerChar) {
    if (!satelliteView) return;
    
    // 1. Get the board pointer from MySatelliteView
    // FIXED: getBoardReference() now returns const GameBoard*, so we just cast away const
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

std::vector<std::pair<size_t, size_t>> MyBattleInfo::getOpponents() const {
    return knownEnemyLocations; // Return the list of known enemy locations
}

GameBoard* MyBattleInfo::getGameBoard() const {
    return gameBoard; // Return the pointer to the game board
} // FIXED: Removed extra semicolon
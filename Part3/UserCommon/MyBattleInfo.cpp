// MyBattleInfo.cpp - Implementation file
#include "MyBattleInfo.h"

MyBattleInfo::MyBattleInfo(MySatelliteView* satelliteView, char myPlayerChar, std::vector<std::shared_ptr<Tank>> myTanks)
    : myPlayerChar(myPlayerChar), myTanks(std::move(myTanks)) {
    if (!satelliteView) return;

    gameBoard = const_cast<Algorithm_211466123_212399455::GameBoard*>(satelliteView->getBoardReference());
    knownEnemyLocations.clear();
    char enemyChar = (myPlayerChar == '1') ? '2' : '1';

    size_t max_width = static_cast<size_t>(gameBoard->getWidth());
    size_t max_height = static_cast<size_t>(gameBoard->getHeight());

    for (size_t y = 0; y < max_height; ++y) {
        for (size_t x = 0; x < max_width; ++x) {
            char cellContent = satelliteView->getObjectAt(x, y);
            if (cellContent == enemyChar) {
                knownEnemyLocations.emplace_back(x, y);
            }
        }
    }
}


std::vector<std::pair<size_t, size_t>> MyBattleInfo::getOpponents() const {
    return knownEnemyLocations; // Return the list of known enemy locations
}

Algorithm_211466123_212399455::GameBoard* MyBattleInfo::getGameBoard() const {
    return gameBoard; // Return the pointer to the game board
} 


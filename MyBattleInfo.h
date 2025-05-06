#pragma once

#include "BattleInfo.h"
#include "SatelliteView.h"
#include <vector>
#include <string>
#include <utility>

class MyBattleInfo : public BattleInfo {
public:
    // Tank-specific fields
    size_t x = 0;
    size_t y = 0;
    int direction = 0;
    size_t shellsRemaining = 0;
    int tankIndex = 0;

    // Battlefield awareness
    std::vector<std::string> satelliteMatrix;
    std::vector<std::pair<size_t, size_t>> knownEnemyLocations;
    std::vector<std::pair<size_t, size_t>> knownMineLocations;
    std::string tacticalMessage;

    // Constructors
    MyBattleInfo();
    MyBattleInfo(size_t x, size_t y, int dir, size_t shells, int tankIdx);

    // Extract battlefield view
    void extractSatelliteMatrix(const SatelliteView& view, size_t rows, size_t cols);
};

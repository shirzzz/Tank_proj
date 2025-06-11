#ifndef PLAYER_H
#define PLAYER_H
#pragma once
#include "TankAlgorithm.h"
#include "SatelliteView.h"

class Player {
protected:
    int player_index_;
    size_t x_, y_, max_steps_, num_shells_;

    
public:
    Player(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells)
        : player_index_(player_index), x_(x), y_(y), max_steps_(max_steps), num_shells_(num_shells) {}
    virtual ~Player() {}
    virtual void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) = 0;
        int getPlayerIndex() const { return player_index_; }
};

#endif //PLAYER_H
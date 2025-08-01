//Player2.h
#ifndef PLAYER2_H
#define PLAYER2_H
#pragma once
#include "common/Player.h"         
#include "Tank.h"           
#include <vector>          
#include <memory>
#include <algorithm>

namespace Algorithm_211466123_212399455{

class Player2 : public Player {
private:
    size_t num_tanks = 0;
    std::vector<std::shared_ptr<Tank>> tanks;
    std::vector<std::unique_ptr<TankAlgorithm>> tank_algorithms;

public:
    // Default constructor
    Player2() : Player(0, 0, 0, 0, 0), num_tanks(0) {}
    
    // ADDED: Delete copy operations (because of unique_ptr)
    Player2(const Player2&) = delete;
    Player2& operator=(const Player2&) = delete;
    
    // ADDED: Default move operations
    Player2(Player2&&) = default;
    Player2& operator=(Player2&&) = default;
    
    // Parameterized constructor
    Player2(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells, size_t num_tanks)
        : Player(player_index, x, y, max_steps, num_shells), num_tanks(num_tanks) {}

    void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) override;
    
    void addTank(std::shared_ptr<Tank> tank) {
        tanks.push_back(std::move(tank));
    }

    void addTankAlgorithm(std::unique_ptr<TankAlgorithm> algorithm) {
        tank_algorithms.push_back(std::move(algorithm));
    }
    
    const std::vector<std::shared_ptr<Tank>>& getTanks() const {
        return tanks;
    }
    
    std::vector<std::unique_ptr<TankAlgorithm>>& getTankAlgorithms() {
        return tank_algorithms;
    }
    
    void setNumTanks(size_t num) {
        num_tanks = num;
    }
    
    size_t getNumTanks() const {
        return num_tanks;
    }
    
    // bool isTankKilled(int index) const {
    //     return killed_tanks[index];
    // }
    
    // int getNumKilledTanks() const {
    //     return num_killed_tanks;
    // }
    
    // size_t getSumShells() const {
    //     return sum_shells;
    // }
    
    // void player2Shoot() {
    //     sum_shells--;
    // }
    
    // void removeTank(std::shared_ptr<Tank> tank) {
    //     auto it = std::find(tanks.begin(), tanks.end(), tank);
    //     if (it != tanks.end()) {
    //         tanks.erase(it);
    //     }
    // }
};

#endif //PLAYER2_H
}
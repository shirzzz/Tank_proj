//Player1
#ifndef PLAYER1_H
#define PLAYER1_H
#pragma once
#include "Player.h"         
#include "../Tank.h"           
#include <vector>          
#include <memory>
#include <algorithm>

class Player1 : public Player {
private:
    size_t num_tanks = 0;
    size_t sum_shells = 0; // Total number of shells available for all tanks
    std::vector<bool> killed_tanks;
    int num_killed_tanks = 0;
    std::vector<std::shared_ptr<Tank>> tanks;
    std::vector<std::unique_ptr<TankAlgorithm>> tank_algorithms;

public:
    // Default constructor
    Player1() : Player(0, 0, 0, 0, 0), num_tanks(0) {}
    Player1(const Player1&) = delete;
    Player1& operator=(const Player1&) = delete;
    //Default move constructor and move assignment operator
    Player1(Player1&&) = default;
    Player1& operator=(Player1&&) = default;
    // Parameterized constructor
    Player1(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells, size_t num_tanks)
        : Player(player_index, x, y, max_steps, num_shells), num_tanks(num_tanks) {}

    void updateTankWithBattleInfo([[maybe_unused]] TankAlgorithm& tank, 
                                 [[maybe_unused]] SatelliteView& satellite_view) override {
    }
    
    void addTank(std::shared_ptr<Tank> tank) {
        tanks.push_back(std::move(tank));
    }

    void addTankAlgorithm(std::unique_ptr<TankAlgorithm> algorithm) {
        tank_algorithms.push_back(std::move(algorithm));
    }
    
    const std::vector<std::shared_ptr<Tank>>& getTanks() const {
        return tanks;
    }
    const std::vector<std::unique_ptr<TankAlgorithm>>& getTankAlgorithms() {
        return tank_algorithms;
    }
    
    void setNumTanks(size_t num) {
        killed_tanks.resize(num);
        num_tanks = num;
    }
    
    size_t getNumTanks() const {
        return num_tanks;
    }
    
    void addKilledTank(int index) {
        killed_tanks[index] = true;
    }
    
    bool isTankKilled(int index) const {
        return killed_tanks[index];
    }
    
    int getNumKilledTanks() const {
        return num_killed_tanks;
    }
    
    void setNumKilledTanks(int num) {
        num_killed_tanks = num;
    }
    size_t getSumShells() const {
        return sum_shells; // Return the total number of shells available for all tanks
    }
    void player1Shoot() {
        sum_shells--; // Decrease the total number of shells available for all tanks
    }
    void removeTank(std::shared_ptr<Tank> tank) {
        auto it = std::find(tanks.begin(), tanks.end(), tank);
        if (it != tanks.end()) {
            tanks.erase(it);
        }
    }
};

#endif //PLAYER1_H
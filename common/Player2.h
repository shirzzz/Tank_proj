//Player2
#ifndef PLAYER2_H
#define PLAYER2_H
#pragma once
#include "Player.h"         
#include "../Tank.h"           
#include <vector>          
#include <memory>

class Player2 : public Player {
private:
    size_t num_tanks = 0;
    std::vector<bool> killed_tanks;
    size_t sum_shells = 0; // Total number of shells available for all tanks
    int num_killed_tanks = 0;
    std::vector<std::shared_ptr<Tank>> tanks;
    std::vector<TankAlgorithm*> tank_algorithms; // Vector to hold tank algorithms

public:
    // Default constructor
    Player2() : Player(0, 0, 0, 0, 0), num_tanks(0) {}
    
    // Parameterized constructor
    Player2(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells, size_t num_tanks)
        : Player(player_index, x, y, max_steps, num_shells), num_tanks(num_tanks) {}

    void updateTankWithBattleInfo([[maybe_unused]] TankAlgorithm& tank, 
                                 [[maybe_unused]] SatelliteView& satellite_view) override {
        // Implement the logic to update the tank with battle information
    }
    
    void addTank(std::shared_ptr<Tank> tank) {
        tanks.push_back(tank);
    }

    void addTankAlgorithm(TankAlgorithm* algorithm) {
        tank_algorithms.push_back(algorithm);
    }

    std::vector<TankAlgorithm*> getTankAlgorithms() const {
        return tank_algorithms;
    }
    
    std::vector<std::shared_ptr<Tank>> getTanks() const {
        return tanks;
    }
    
    void setNumTanks(size_t num) {
        killed_tanks.resize(num);  // Added this line like in Player1
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
        return sum_shells;
    }
    void player2Shoot() {
        sum_shells --; // Decrease the total number of shells available for all tanks
    }
    void removeTank(std::shared_ptr<Tank> tank) {
        auto it = std::find(tanks.begin(), tanks.end(), tank);
        if (it != tanks.end()) {
            tanks.erase(it);
        }
    }
};

#endif //PLAYER2_H
#ifndef PLAYER1_H
#define PLAYER1_H
#pragma once
#include "../common/Player.h"         
#include "../UserCommon/Tank.h"           
#include <vector>          
#include <memory>
#include <algorithm>
namespace Algorithm_211466123_212399455{
class Player1 : public Player {
private:
    size_t num_tanks = 0;

    std::vector<std::shared_ptr<Tank>> tanks;
    std::vector<std::unique_ptr<TankAlgorithm>> tank_algorithms;

public:
    // Default constructor
    Player1() : Player(0, 0, 0, 0, 0), num_tanks(0) {}
    
    // Delete copy operations (because of unique_ptr)
    Player1(const Player1&) = delete;
    Player1& operator=(const Player1&) = delete;
    
    // Default move operations
    Player1(Player1&&) = default;
    Player1& operator=(Player1&&) = default;
    
    // Parameterized constructor
    Player1(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells, size_t num_tanks)
        : Player(player_index, x, y, max_steps, num_shells), num_tanks(num_tanks) {}

    //  Only declaration here - implementation in .cpp file
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
    
    //Removed const from return type
    std::vector<std::unique_ptr<TankAlgorithm>>& getTankAlgorithms() {
        return tank_algorithms;
    }
    
    void setNumTanks(size_t num) {
        num_tanks = num;
    }
    
    size_t getNumTanks() const {
        return num_tanks;
    }
    
    // void addKilledTank(int index) {
    //     killed_tanks[index] = true;
    // }
    
    // bool isTankKilled(int index) const {
    //     return killed_tanks[index];
    // }
    
    // int getNumKilledTanks() const {
    //     return num_killed_tanks;
    // }
    
    // void setNumKilledTanks(int num) {
    //     num_killed_tanks = num;
    // }
    
    // size_t getSumShells() const {
    //     return sum_shells;
    // }
    
    // void player1Shoot() {
    //     sum_shells--;
    // }
    
    // void removeTank(std::shared_ptr<Tank> tank) {
    //     auto it = std::find(tanks.begin(), tanks.end(), tank);
    //     if (it != tanks.end()) {
    //         tanks.erase(it);
    //     }
    // }
};

#endif //PLAYER1_H
} // namespace Algorithm_211466123_212399455
//Player2
#ifndef PLAYER2_H
#define PLAYER2_H
#pragma once
#include "Player.h"         
#include "Tank.h"           
#include <vector>          
#include <memory>

class Player2 : public Player {
private:
    int player_index;
    size_t x, y;
    size_t max_steps;
    size_t num_shells;
    size_t num_tanks = 0;
    std::vector<bool> killed_tanks;
    int num_killed_tanks = 0;
    std::vector<std::shared_ptr<Tank>> tanks;
public:
    Player2(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells, size_t num_tanks)
        : Player(player_index, x, y, max_steps, num_shells), num_tanks(num_tanks) {}

    void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) override {
        // Implement the logic to update the tank with battle information
    }
    void addTank(std::shared_ptr<Tank> tank) {
        tanks.push_back(tank);
    }
    std::vector<std::shared_ptr<Tank>> getTanks() const {
        return tanks;
    }
    void setNumTanks(size_t num) {
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
};

#endif //PLAYER2_H
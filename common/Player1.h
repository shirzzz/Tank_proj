//Player1
#ifndef PLAYER1_H
#define PLAYER1_H
#pragma once

class Player1 : public Player {
private:
    int player_index;
    size_t x;
    size_t y;
    size_t max_steps;
    size_t num_shells;
    std::vector<std::shared_ptr<Tank>> tanks;
public:
    Player1(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells)
        : Player(player_index, x, y, max_steps, num_shells) {}

    void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) override {
        // Implement the logic to update the tank with battle information
    }
    void addTank(std::shared_ptr<Tank> tank) {
        tanks.push_back(tank);
    }
    std::vector<std::shared_ptr<Tank>> getTanks() const {
        return tanks;
    }
};

#endif //PLAYER1_H
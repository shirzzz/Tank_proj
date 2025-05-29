#ifndef MYTANKALGORITHMFACTORY_H
#define MYTANKALGORITHMFACTORY_H

#include "TankAlgorithmFactory.h"
#include "Player1Algorithm.h"
#include "Player2Algorithm.h"
#include <memory>

class MyTankAlgorithmFactory : public TankAlgorithmFactory {
public:
    std::unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const override {
        if (player_index == 1) {
            return std::make_unique<Player1Algorithm>();
        } else if (player_index == 2) {
            return std::make_unique<Player2Algorithm>();
        }
        return nullptr;
    }
};

#endif // MYTANKALGORITHMFACTORY_H
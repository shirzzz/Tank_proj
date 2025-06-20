#ifndef MYTANKALGORITHMFACTORY_H
#define MYTANKALGORITHMFACTORY_H

#include "common/TankAlgorithmFactory.h"
#include "BfsChaserShir.h"
#include "Chased.h"
#include <memory>

class MyTankAlgorithmFactory : public TankAlgorithmFactory {
public:
    virtual ~MyTankAlgorithmFactory() {}
    
    // Implementation of the abstract factory method
    std::unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const override {
        if (player_index % 2 == 1) {
            // Player 1 (odd numbers) gets BfsChaserShir
            return std::make_unique<BfsChaserShir>(player_index, tank_index);
        } else {
            // Player 2 (even numbers) gets Chased
            return std::make_unique<Chased>(player_index, tank_index);
        }
    }
};

#endif // MYTANKALGORITHMFACTORY_H
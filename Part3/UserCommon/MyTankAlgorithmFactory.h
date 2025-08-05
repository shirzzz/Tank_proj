#ifndef MYTANKALGORITHMFACTORY_H
#define MYTANKALGORITHMFACTORY_H

#include "../common/TankAlgorithmFactory.h"
#include "../Algorithm/BfsChaserShir.h"
#include "../Chased.h"
#include <memory>

class MyTankAlgorithmFactory : public TankAlgorithmFactory {
public:
    virtual ~MyTankAlgorithmFactory() {}
    
    // Implementation of the abstract factory method
    std::unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const override {
        if (player_index == 1) {
            return std::make_unique<Algorithm_211466123_212399455::BfsChaserShir>(player_index, tank_index);
        } else if (player_index == 2) {
            return std::make_unique<Chased>(player_index, tank_index);
        }
        return nullptr; // Invalid player index
    }
};

#endif // MYTANKALGORITHMFACTORY_H
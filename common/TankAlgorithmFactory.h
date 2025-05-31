#ifndef TANKALGORITHMFACTORY_H
#define TANKALGORITHMFACTORY_H

#include <memory>
#include "TankAlgorithm.h"

class TankAlgorithmFactory {
public:
    virtual ~TankAlgorithmFactory() {}
    virtual std::unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const = 0;
};

#endif // TANKALGORITHMFACTORY_H
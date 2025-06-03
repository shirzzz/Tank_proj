#pragma once

#include "TankAlgorithmFactory.h"
#include "../BfsChaserShir.h"
#include "../Chased.h"

#include <memory>
class TankAlgorithmFactory : public TankAlgorithmFactory
{
public:
    // === Destructor === //
    ~MyTankAlgorithmFactory() override = default;

    // === Producing TankAlgorithm objects === //
    std::unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const override
    {
        if (player_index % 2 == 1)
            return std::make_shared<BfsChaserShir>(player_index, tank_index);

        return std::make_shared<Chased>(player_index, tank_index);
    }
};
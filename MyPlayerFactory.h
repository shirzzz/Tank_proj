#ifndef MYPLAYERFACTORY_H
#define MYPLAYERFACTORY_H

#include "common/PlayerFactory.h"
#include "common/Player1.h"
#include "common/Player2.h"
#include <memory>

class MyPlayerFactory : public PlayerFactory {
public:
    virtual ~MyPlayerFactory() {}
    
    // Implementation of the abstract factory method
    std::unique_ptr<Player> create(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells) const override {
        if (player_index == 1) {
            return std::make_unique<Player1>(player_index, x, y, max_steps, num_shells, 0);
        } else if (player_index == 2) {
            return std::make_unique<Player2>(player_index, x, y, max_steps, num_shells, 0);
        }
        return nullptr; // Invalid player index
    }
};

#endif // MYPLAYERFACTORY_H
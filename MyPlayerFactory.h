#ifndef MYPLAYERFACTORY_H
#define MYPLAYERFACTORY_H

#include "PlayerFactory.h"
#include "Player1.h"
#include "Player2.h"
#include <memory>

class MyPlayerFactory : public PlayerFactory {
public:
    std::unique_ptr<Player> create(int player_index, size_t x, size_t y, 
                                 size_t max_steps, size_t num_shells) const override {
        if (player_index == 1) {
            return std::make_unique<Player1>(player_index, x, y, max_steps, num_shells, 0);
        } else if (player_index == 2) {
            return std::make_unique<Player2>(player_index, x, y, max_steps, num_shells, 0);
        }
        return nullptr;
    }
};

#endif // MYPLAYERFACTORY_H
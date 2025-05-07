//PlayerFactory.h
#ifndef PLAYERFACTORY_H
#define PLAYERFACTORY_H
#pragma once

class PlayerFactory {
public:
    virtual ~PlayerFactory() {} // Virtual destructor for proper cleanup of derived classes
    virtual unique_ptr<Player> create(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells) const = 0;
};

#endif //PLAYERFACTORY_H



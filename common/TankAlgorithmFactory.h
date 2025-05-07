//TankAlgorithmFactory.h
#ifndef TANKALGORITHMFACTORY_H
#define TANKALGORITHMFACTORY_H
#pragma once

class TankAlgorithmFactory {
public:
    virtual ~TankAlgorithmFactory() {} // Virtual destructor for proper cleanup of derived classes
    virtual unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const = 0; //notice we start from 0
};

#endif //TANKALGORITHMFACTORY_H



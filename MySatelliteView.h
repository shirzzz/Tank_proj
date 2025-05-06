#pragma once

#include "SatelliteView.h"
#include "GameBoard.h"

class MySatelliteView : public SatelliteView {
private:
    const GameBoard& board;

public:
    explicit MySatelliteView(const GameBoard& board);
    char getObjectAt(size_t x, size_t y) const override;
};

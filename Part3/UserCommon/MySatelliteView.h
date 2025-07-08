#pragma once
#include <cstddef>
#include "common/SatelliteView.h"
#include "../Algorithm/GameBoard.h"

class MySatelliteView : public SatelliteView {
private:
    const GameBoard& board;

public:
    explicit MySatelliteView(const GameBoard& board);
    char getObjectAt(size_t x, size_t y) const override;
    const GameBoard* getBoardReference() const {
    return &board;
}
};

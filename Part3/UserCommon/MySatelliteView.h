#pragma once
#include <cstddef>
#include "../common/SatelliteView.h"
#include "../Algorithm/GameBoard.h"

class MySatelliteView : public SatelliteView {
private:
    const Algorithm_211466123_212399455::GameBoard& board;

public:
    explicit MySatelliteView(const Algorithm_211466123_212399455::GameBoard& board);
    char getObjectAt(size_t x, size_t y) const override;
    const Algorithm_211466123_212399455::GameBoard* getBoardReference() const {
    return &board;
}
};

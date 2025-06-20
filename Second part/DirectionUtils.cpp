
#include "DirectionUtils.h"
#include "GameBoard.h"

std::pair<int, int> directionToVector(CanonDirection dir) {
    switch (dir) {
        case CanonDirection::U: return {0, -1};
        case CanonDirection::UR: return {1, -1};
        case CanonDirection::R: return {1, 0};
        case CanonDirection::DR: return {1,1};
        case CanonDirection::D: return {0, 1};
        case CanonDirection::DL: return {-1, 1};
        case CanonDirection::L: return {-1, 0};
        case CanonDirection::UL: return {-1, -1};
        default: return {0, 0};
    }
}

CanonDirection getDirectionFromDelta(int dx, int dy) {
    if (dx != 0) dx /= std::abs(dx);
    if (dy != 0) dy /= std::abs(dy);

    if      (dx == 0 && dy == -1) return CanonDirection::U;
    else if (dx == 1 && dy == -1) return CanonDirection::UR;
    else if (dx == 1 && dy == 0)  return CanonDirection::R;
    else if (dx == 1 && dy ==1)  return CanonDirection::DR;
    else if (dx == 0 && dy == 1)  return CanonDirection::D;
    else if (dx == -1 && dy ==1) return CanonDirection::DL;
    else if (dx == -1 && dy == 0) return CanonDirection::L;
    else if (dx == -1 && dy == -1)return CanonDirection::UL;
    return CanonDirection::U; // fallback
}
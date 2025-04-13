
std::pair<int, int> directionToVector(Direction dir) {
    switch (dir) {
        case Direction::UP: return {0, -1};
        case Direction::DOWN: return {0, 1};
        case Direction::LEFT: return {-1, 0};
        case Direction::RIGHT: return {1, 0};
        case Direction::UP_LEFT: return {-1, -1};
        case Direction::UP_RIGHT: return {1, -1};
        case Direction::DOWN_LEFT: return {-1, 1};
        case Direction::DOWN_RIGHT: return {1, 1};
        default: return {0, 0};
    }
}

Direction getDirectionFromDelta(int dx, int dy) {
    if (dx != 0) dx /= std::abs(dx);
    if (dy != 0) dy /= std::abs(dy);

    if      (dx == 0 && dy == -1) return Direction::UP;
    else if (dx == 1 && dy == -1) return Direction::UP_RIGHT;
    else if (dx == 1 && dy == 0)  return Direction::RIGHT;
    else if (dx == 1 && dy == 1)  return Direction::DOWN_RIGHT;
    else if (dx == 0 && dy == 1)  return Direction::DOWN;
    else if (dx == -1 && dy == 1) return Direction::DOWN_LEFT;
    else if (dx == -1 && dy == 0) return Direction::LEFT;
    else if (dx == -1 && dy == -1)return Direction::UP_LEFT;

    return Direction::UP; // fallback
}
#include "Direction.h"
#include <utility>
#include "CanonDirection.h"
std::pair<int, int> directionToVector(Direction dir);
CanonDirection getDirectionFromDelta(int dx, int dy);
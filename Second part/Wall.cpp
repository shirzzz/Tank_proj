#include "Shape.h"
#include "Wall.h"
Wall::Wall(int x, int y): Shape(x, y), lives(2) {
    this->cell_type = CellType::WALL;
} 


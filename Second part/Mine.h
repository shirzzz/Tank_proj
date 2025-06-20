//
// Created by Owner on 24/04/2025.
//

#ifndef MINE_H
#define MINE_H
#include "Shape.h"


class Mine : public Shape {
public:
    Mine(int x, int y) : Shape(x, y) {
        this->cell_type = CellType::MINE;
    }
};



#endif //MINE_H

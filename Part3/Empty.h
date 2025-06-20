//
// Created by Owner on 24/04/2025.
//

#ifndef EMPTY_H
#define EMPTY_H
#include "Shape.h"


class Empty : public Shape {
public:
    Empty(int x, int y) : Shape(x, y) {
        this->cell_type = CellType::EMPTY;
    }
    ~Empty() override = default;
    bool isEmpty() const { return true; } // Always returns true for Empty cells

};



#endif //EMPTY_H

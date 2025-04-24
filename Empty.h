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
    // Additional methods or properties can be added here
    // For example, if you want to add a method to check if the cell is empty
    bool isEmpty() const { return true; } // Always returns true for Empty cells

};



#endif //EMPTY_H

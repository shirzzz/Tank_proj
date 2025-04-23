//
// Created by Owner on 01/04/2025.
//

#pragma once

#include "CanonDirection.h"
#include "shape.h"

class Shell : public Shape {
    int speed;

private:
    std::pair<int, int> previousPosition;

public:
    Shell(int x, int y, CanonDirection cdir);
    void storePreviousPosition() { previousPosition = {x, y}; }
    std::pair<int, int> getPreviousPosition() const { return previousPosition; }


};





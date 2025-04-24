//
// Created by Owner on 01/04/2025.
//

#pragma once

#include "CanonDirection.h"
#include "shape.h"

class Shell : public Shape {
private:
    std::pair<int, int> previousPosition;
    int speed;
    CanonDirection canon_dir;

public:
    Shell(int x, int y, CanonDirection cdir);
    void storePreviousPosition() { previousPosition = {x, y}; }
    std::pair<int, int> getPreviousPosition() const { return previousPosition; }
    CanonDirection getDirection() const { return canon_dir; }


};





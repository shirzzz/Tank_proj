//
// Created by Owner on 01/04/2025.
//

#pragma once

#include "CanonDirection.h"
#include "Shape.h"

class Shell : public Shape {
private:
    std::pair<int, int> previousPosition;
    int speed = 1 ;
    CanonDirection canon_dir= CanonDirection::U; // Default direction

public:
    Shell(int x, int y, CanonDirection cdir);
    void storePreviousPosition() { previousPosition = {x, y}; }
    std::pair<int, int> getPreviousPosition() const { return previousPosition; }
    CanonDirection getDirection() const { return canon_dir; }


};





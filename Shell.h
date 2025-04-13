//
// Created by Owner on 01/04/2025.
//

#ifndef SHELL_H
#define SHELL_H
#include "CanonDirection.h"
#include "shape.h"


class Shell : public Shape {
    CanonDirection canon_dir;
    int speed;
public:
    Shell(int x, int y, int direction, CanonDirection cdir);
};






#endif //SHELL_H

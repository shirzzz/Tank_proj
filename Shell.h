//
// Created by Owner on 01/04/2025.
//

#ifndef SHELL_H
#define SHELL_H
#include "canon_direction.h"
#include "shape.h"


class Shell : public Shape {
    canon_direction canon_dir;
    int speed;
public:
    Shell(int x, int y, int direction, canon_direction cdir);
};






#endif //SHELL_H

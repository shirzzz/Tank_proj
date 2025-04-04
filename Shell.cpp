//
// Created by Owner on 01/04/2025.
//

#include "Shell.h"
#include "canon_direction.h"
#include "shape.h"

Shell::Shell(int x, int y, int direction, canon_direction cdir) : Shape(x, y, direction), canon_dir(cdir), speed(1){};

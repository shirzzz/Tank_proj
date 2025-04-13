//
// Created by Owner on 01/04/2025.
//

#include "Shell.h"
#include "CanonDirection.h"
#include "shape.h"

Shell::Shell(int x, int y, Direction direction, CanonDirection cdir) : Shape(x, y, direction), canon_dir(cdir), speed(1){};

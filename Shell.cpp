#include "Shell.h"
#include "CanonDirection.h"
#include "shape.h"

// Constructs a shell with initial position and canon direction
Shell::Shell(int x, int y, CanonDirection cdir) : Shape(x, y), canon_dir(cdir) {
    this->cell_type = CellType::SHELL;
}
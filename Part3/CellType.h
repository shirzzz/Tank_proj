#ifndef CELLTYPE_H
#define CELLTYPE_H
#include <string>
#include <ostream>
    enum class CellType {
        EMPTY,
        WALL,
        TANK1,
        TANK2,
        SHELL,
        MINE
    };


    inline std::string toString(CellType cell) {
        switch (cell) {
            case CellType::EMPTY: return "EMPTY";
            case CellType::WALL: return "WALL";
            case CellType::TANK1: return "TANK1";
            case CellType::TANK2: return "TANK2";
            case CellType::SHELL: return "SHELL";
            case CellType::MINE: return "MINE";
            default: return "BAD_CELL";
        }
    }
    
    inline std::ostream& operator<<(std::ostream& os, CellType action) {
        return os << toString(action);
    }
#endif //CELLTYPE_H

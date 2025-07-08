
#ifndef CANON_DIRECTION_H
#define CANON_DIRECTION_H
#include <string>
#include <ostream>
enum class CanonDirection {
    U,
    UR,
    R,
    DR,
    D,
    DL,
    L,
    UL
};

inline std::string toString(CanonDirection direction) {
    switch (direction) {
        case CanonDirection::U: return "U";
        case CanonDirection::UR: return "UR";
        case CanonDirection::R: return "R";
        case CanonDirection::DR: return "DR";
        case CanonDirection::D: return "D";
        case CanonDirection::DL: return "DL";
        case CanonDirection::L: return "L";
        case CanonDirection::UL: return "UL";
        default: return "BAD_DIRECTION";
    }
}

inline std::ostream& operator<<(std::ostream& os, CanonDirection action) {
    return os << toString(action);
}
#endif //CANON_DIRECTION_H
#ifndef DESTRUCTIONCAUSE_H
#define DESTRUCTIONCAUSE_H


enum class DestructionCause {
    NONE,
    SHELL,
    MINE,
    TANK,
    SHELLOPPONENT,
    MINEOPPONENT,
    MULTIPLE,
    OUTOFAMMO,
};

inline std::string toString(DestructionCause cause) {
    switch (cause) {
        case DestructionCause::NONE: return "NONE";
        case DestructionCause::SHELL: return "SHELL HIT ME";
        case DestructionCause::MINE: return "I GOT ONTO A MINE";
        case DestructionCause::TANK: return "TANK COLLISION";
        case DestructionCause::SHELLOPPONENT: return "I HIT OPPONENT WITH A SHELL";
        case DestructionCause::MINEOPPONENT: return "MY OPPONENT GOT TO A MINE";
        case DestructionCause::MULTIPLE: return "MULTIPLE";
        case DestructionCause::OUTOFAMMO: return "Out of ammo";
        default: return "UNKNOWN";
    }
}
inline std::ostream& operator<<(std::ostream& os, DestructionCause cause) {
    return os << toString(cause);
}

#endif
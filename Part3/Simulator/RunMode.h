
#ifndef RUNMODE_H
#define RUNMODE_H
#include <string>
#include <ostream>


enum class RunMode {
    Comparative,  // Run comparative mode with two algorithms on the same map
    Competitive,  // Run competitive mode with multiple algorithms on multiple maps
    Invalid       // Invalid mode, used for error handling
};

inline std::string toString(RunMode mode) {
    switch (mode) {
        case RunMode::Comparative: return "Comparative";
        case RunMode::Competitive: return "Competitive";
        case RunMode::Invalid: return "Invalid";
        default: return "Unknown";
    }
}

inline std::ostream& operator<<(std::ostream& os, RunMode action) {
    return os << toString(action);
};

#endif //RUNMODE_H

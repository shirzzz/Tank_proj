// #include "ActionType.h"
// #include <string>
// #include <ostream>
//
// inline std::string toString(ActionType action) {
//     switch (action) {
//         case ActionType::MOVE_FORWARD: return "MOVE_FORWARD";
//         case ActionType::MOVE_BACKWARD: return "MOVE_BACKWARD";
//         case ActionType::ROTATE_EIGHTH_LEFT: return "ROTATE_EIGHTH_LEFT";
//         case ActionType::ROTATE_EIGHTH_RIGHT: return "ROTATE_EIGHTH_RIGHT";
//         case ActionType::ROTATE_QUARTER_LEFT: return "ROTATE_QUARTER_LEFT";
//         case ActionType::ROTATE_QUARTER_RIGHT: return "ROTATE_QUARTER_RIGHT";
//         case ActionType::SHOOT: return "SHOOT";
//         case ActionType::INVALID_ACTION: return "INVALID_ACTION";
//         case ActionType::SHELL_MOVE_LEFT: return "SHELL_MOVE_LEFT";
//         case ActionType::SHELL_MOVE_RIGHT: return "SHELL_MOVE_RIGHT";
//         case ActionType::SHELL_MOVE_UP: return "SHELL_MOVE_UP";
//         case ActionType::SHELL_MOVE_DOWN: return "SHELL_MOVE_DOWN";
//         case ActionType::SHELL_MOVE_UP_LEFT: return "SHELL_MOVE_UP_LEFT";
//         case ActionType::SHELL_MOVE_UP_RIGHT: return "SHELL_MOVE_UP_RIGHT";
//         case ActionType::SHELL_MOVE_DOWN_LEFT: return "SHELL_MOVE_DOWN_LEFT";
//         case ActionType::SHELL_MOVE_DOWN_RIGHT: return "SHELL_MOVE_DOWN_RIGHT";
//         case ActionType::WIN: return "WIN";
//         case ActionType::LOSE: return "LOSE";
//         case ActionType::DRAW: return "DRAW";
//         default: return "BAD_STEP";
//     }
// }
//
// inline std::ostream& operator<<(std::ostream& os, ActionType action) {
//     return os << toString(action);
// }
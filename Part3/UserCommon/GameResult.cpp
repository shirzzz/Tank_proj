#include <../common/GameResult.h>
#include <vector>
#include <stdexcept>
//setters and getters for GameResult
inline void setWinner(GameResult& result, int winner) {
    result.winner = winner;
}
inline int getWinner(const GameResult& result) {
    return result.winner;
}
inline void setReason(GameResult& result, GameResult::Reason reason) {
    result.reason = reason;
}
inline GameResult::Reason getReason(const GameResult& result) {
    return result.reason;
}
inline void addRemainingTank(GameResult& result, size_t tank_index) {
    result.remaining_tanks.push_back(tank_index);
}
inline const std::vector<size_t>& getRemainingTanks(const GameResult& result) {
    return result.remaining_tanks;
}
inline void clearRemainingTanks(GameResult& result) {
    result.remaining_tanks.clear();
}
inline size_t getRemainingTankCount(const GameResult& result) {
    return result.remaining_tanks.size();
}
inline size_t getRemainingTankAt(const GameResult& result, size_t index) {
    if (index < result.remaining_tanks.size()) {
        return result.remaining_tanks[index];
    }
    throw std::out_of_range("Index out of range for remaining tanks.");
}
inline void setRemainingTankAt(GameResult& result, size_t index, size_t tank_index) {
    if (index < result.remaining_tanks.size()) {
        result.remaining_tanks[index] = tank_index;
    } else {
        throw std::out_of_range("Index out of range for remaining tanks.");
    }
}
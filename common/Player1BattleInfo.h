#ifndef PLAYER1BATTLEINFO_H
#define PLAYER1BATTLEINFO_H

// *** ADDED: Required includes to prevent compilation errors ***
#include "BattleInfo.h"     // WHY: Player1BattleInfo inherits from BattleInfo
#include "Tank.h"           // WHY: Player1BattleInfo has std::vector<Tank>
#include "GameBoard.h"      // WHY: Player1BattleInfo has std::shared_ptr<GameBoard>
#include <vector>           // WHY: Player1BattleInfo uses std::vector
#include <memory>           // WHY: Player1BattleInfo uses std::shared_ptr

class Player1BattleInfo : public BattleInfo {
public:
    std::vector<Tank> opponents;
    std::shared_ptr<GameBoard> board;
};

#endif // PLAYER1BATTLEINFO_H
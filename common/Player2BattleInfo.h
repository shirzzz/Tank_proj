#ifndef PLAYER2BATTLEINFO_H
#define PLAYER2BATTLEINFO_H

// *** ADDED: Required includes to prevent compilation errors ***
#include "BattleInfo.h"     // WHY: Player2BattleInfo inherits from BattleInfo
#include "Tank.h"           // WHY: Player2BattleInfo has std::vector<Tank>
#include "GameBoard.h"      // WHY: Player2BattleInfo has std::shared_ptr<GameBoard>
#include <vector>           // WHY: Player2BattleInfo uses std::vector
#include <memory>           // WHY: Player2BattleInfo uses std::shared_ptr

class Player2BattleInfo : public BattleInfo {
public:
    std::vector<Tank> opponents;
    std::shared_ptr<GameBoard> board;
};

#endif // PLAYER2BATTLEINFO_H
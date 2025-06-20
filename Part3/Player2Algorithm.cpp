// #include "Player2Algorithm.h"
// #include "Player2BattleInfo.h"
// #include "Chased.h"

// ActionRequest Player2Algorithm::getAction() {
//     if (!current_tank || opponent_tanks.empty() || !board) {
//         return ActionRequest::GetBattleInfo;
//     }
    
//     Chased chased;
//     return chased.decideNextAction(*board, *current_tank, opponent_tanks[0]);
// }

// void Player2Algorithm::updateBattleInfo(BattleInfo& battleInfo) {
//     auto& myInfo = dynamic_cast<Player2BattleInfo&>(battleInfo);
//     opponent_tanks = myInfo.opponents;
//     board = myInfo.board;
//     // current_tank should be set by the calling context
// }

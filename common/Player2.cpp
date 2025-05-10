#include "Player.h"
void Player2::updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& view) {
    Player2BattleInfo info;
    // You must scan the satellite view and extract useful data
    info.opponents = extractOpponentTanksFromView(view);
    info.board = buildGameBoardFromView(view);  // optional helper
    tank.updateBattleInfo(info);
}
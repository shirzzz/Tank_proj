#include "Player.h"
void Player1::updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& view) {
    Player1BattleInfo info;

    // You must scan the satellite view and extract useful data
    info.opponents = extractOpponentTanksFromView(view);
    info.board = buildGameBoardFromView(view);  // optional helper

    tank.updateBattleInfo(info);
}
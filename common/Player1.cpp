#include "Player1.h"
#include "../MyBattleInfo.h"
#include "../MySatelliteView.h"

void Player1::updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) {
    // Cast SatelliteView to MySatelliteView to access our specific methods
    MySatelliteView* mySatView = dynamic_cast<MySatelliteView*>(&satellite_view);
    if (!mySatView) {
        // Handle error case - satellite view is not MySatelliteView
        std::cerr << "Error: SatelliteView is not MySatelliteView in Player1" << std::endl;
        return;
    }
    
    // Create MyBattleInfo using your constructor
    // Player 1 looks for Player 2 tanks ('2' characters)
    MyBattleInfo battleInfo(mySatView, '1');
    
    // Call the tank algorithm's updateBattleInfo method
    // This passes the battlefield information to the tank
    tank.updateBattleInfo(battleInfo);
    
    // Optional: Log or store coordination information
    std::cout << "Player 1 provided battle info to tank. Found " 
              << battleInfo.knownEnemyLocations.size() << " enemy tanks." << std::endl;
}

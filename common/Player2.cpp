#include "Player2.h" 
#include "../MyBattleInfo.h"
#include "../MySatelliteView.h"

void Player2::updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) {
    // Cast SatelliteView to MySatelliteView to access our specific methods
    MySatelliteView* mySatView = dynamic_cast<MySatelliteView*>(&satellite_view);
    if (!mySatView) {
        // Handle error case - satellite view is not MySatelliteView
        std::cerr << "Error: SatelliteView is not MySatelliteView in Player2" << std::endl;
        return;
    }
    
    // Create MyBattleInfo using your constructor
    // Player 2 looks for Player 1 tanks ('1' characters)
    MyBattleInfo battleInfo(mySatView, '2');
    
    // Call the tank algorithm's updateBattleInfo method
    tank.updateBattleInfo(battleInfo);
    
    // Optional: Log or store coordination information
    std::cout << "Player 2 provided battle info to tank. Found " 
              << battleInfo.knownEnemyLocations.size() << " enemy tanks." << std::endl;
}


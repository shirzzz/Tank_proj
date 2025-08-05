#include "Player1.h"
#include "../UserCommon/MyBattleInfo.h"
#include "../UserCommon/MySatelliteView.h"



void Algorithm_211466123_212399455::Player1::updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) {
    // Cast SatelliteView to MySatelliteView to access our specific methods
    MySatelliteView* mySatView = dynamic_cast<MySatelliteView*>(&satellite_view);
    if (!mySatView) {
        // Handle error case - satellite view is not MySatelliteView
        std::cerr << "Error: SatelliteView is not MySatelliteView in Player1" << std::endl;
        return;
    }
    
    // Pass tanks directly without copying
    MyBattleInfo battleInfo(mySatView, '1', this->tanks);
    
    // Call the tank algorithm's updateBattleInfo method
    // This passes the battlefield information to the tank
    tank.updateBattleInfo(battleInfo);
    
    // Optional: Log or store coordination information
    std::cout << "Player 1 provided battle info to tank. Found " 
              << battleInfo.knownEnemyLocations.size() << " enemy tanks." << std::endl;
}
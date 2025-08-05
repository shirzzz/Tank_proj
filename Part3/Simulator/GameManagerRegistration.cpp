
//#include "../common/GameManagerFactory.h"

//Itai I dont know what to do with this, I just added it 
//I dont know what I am doing

//let's assume it is working
#include "../common/GameManagerRegistration.h"
GameManagerRegistration::GameManagerRegistration(GameManager gamemanager){
    auto& registrar = GameManagerRegistrar::getGameManagerRegistrar();
    registrar.addGameManagerFactory(std::move(gamemanager));
}
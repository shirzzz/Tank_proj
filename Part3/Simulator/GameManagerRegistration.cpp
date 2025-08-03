
//#include "../common/GameManagerFactory.h"

//Itai I dont know what to do with this, I just added it 
//I dont know what I am doing

//let's assume it is working
#include "common/GameManagerRegistration.h"
#include "GameManagerRegistrar.h"

GameManagerRegistration::GameManagerRegistration(GameManagerFactory factory) {
    GameManagerRegistrar::getGameManagerRegistrar().addGameManagerFactoryToLastEntry(std::move(factory));
}
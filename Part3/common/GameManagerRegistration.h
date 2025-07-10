#ifndef GAME_MANAGER_REGISTRATION_H
#define GAME_MANAGER_REGISTRATION_H 
#include <functional>
#include <memory>
#include "AbstractGameManager.h"

struct GameManagerRegistration {
  GameManagerRegistration(std::function<std::unique_ptr<AbstractGameManager>()>);
};

#define REGISTER_GAME_MANAGER(class_name) \
GameManagerRegistration register_me_##class_name \
        ( [] (bool verbose) { return std::make_unique<class_name>(verbose); } );
#endif // GAME_MANAGER_REGISTRATION_H
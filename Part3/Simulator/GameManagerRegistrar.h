#ifndef GAME_MANAGER_REGISTRAR_H
#define GAME_MANAGER_REGISTRAR_H
#include <string>
#include <vector>
#include <memory>
//#include "GameManagerFactory.h" WE DONT HAVE A FACTORY FOR GAMEMANAGER I DONT KNOW WHAT TO DO WITH THIS
#include "../GameManager/GameManager.h"
class GameManagerRegistrar {
    std::vector<GameManagerFactory> game_managers;
    static GameManagerRegistrar registrar;
public:
    static GameManagerRegistrar& getGameManagerRegistrar() {
        return registrar;
    }       
    void addGameManagerFactory(GameManagerFactory&& factory) {
        game_managers.push_back(std::move(factory));
    }
    std::vector<GameManagerFactory>& getGameManagers() {
        return game_managers;
    }
    void removeLast() {
        if (!game_managers.empty()) {
            game_managers.pop_back();
        }
    }
    bool empty() const {
        return game_managers.empty();
    }
    size_t count() const {
        return game_managers.size();
    }
    GameManagerFactory& operator[](size_t index) {
        if (index < game_managers.size()) {
            return game_managers[index];
        }
        throw std::out_of_range("Index out of range in GameManagerRegistrar");
    }
};

#endif // GAME_MANAGER_REGISTRAR_H
#include "../common/PlayerRegistration.h"
#include "AlgorithmRegistrar.h"
#include "../common/PlayerFactory.h"
PlayerRegistration::PlayerRegistration(PlayerFactory factory) {
    auto& regsitrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    regsitrar.addPlayerFactoryToLastEntry(std::move(factory));
}
#include "common/PlayerRegistration.h"
#include "AlgorithmRegistrar.h"

PlayerRegistration::PlayerRegistration(PlayerFactory factory) {
    AlgorithmRegistrar::getAlgorithmRegistrar().addPlayerFactoryToLastEntry(std::move(factory));
}
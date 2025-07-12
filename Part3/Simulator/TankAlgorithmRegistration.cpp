#include "../common/TankAlgorithmRegistration.h"
#include "AlgorithmRegistrar.h"
TankAlgorithmRegistration::TankAlgorithmRegistration(TankAlgorithmFactory factory) {
    auto& regsitrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    regsitrar.addTankAlgorithmFactoryToLastEntry(std::move(factory));
}
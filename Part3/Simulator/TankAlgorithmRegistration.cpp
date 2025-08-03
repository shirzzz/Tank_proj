#include "common/TankAlgorithmRegistration.h"
#include "AlgorithmRegistrar.h"

TankAlgorithmRegistration::TankAlgorithmRegistration(TankAlgorithmFactory factory) {
    AlgorithmRegistrar::getAlgorithmRegistrar().addTankAlgorithmFactoryToLastEntry(std::move(factory));
}
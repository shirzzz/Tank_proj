#ifndef TANK_ALGORITHM_REGISTRATION_H
#define TANK_ALGORITHM_REGISTRATION_H
#include <TankAlgorithmFactory.h>
struct TankAlgorithmRegistration {
  TankAlgorithmRegistration(TankAlgorithmFactory);
};

#define REGISTER_TANK_ALGORITHM(class_name) \
TankAlgorithmRegistration register_me_##class_name \
	( [](int player_index, int tank_index) { \
	    return std::make_unique<class_name>(player_index, tank_index); } \
	);

#endif // TANK_ALGORITHM_REGISTRATION_H
#include "Simulator.h"
#include <iostream>

Simulator::Simulator(bool verbose, size_t num_threads)
    : verbose_(verbose), num_threads_(num_threads) {}

void Simulator::runComparative(const std::string& game_map,
                                const std::string& gmFolder,
                                const std::string& algo1,
                                const std::string& algo2) const {
    std::cout << "Running comparative simulation..." << std::endl;
    // TODO: Load .so files, spin up threads, run games
} 

void Simulator::runCompetitive(const std::string& maps_folder,
                                const std::string& game_manager,
                                const std::string& algorithms_folder) const {
    std::cout << "Running competition simulation..." << std::endl;
    // TODO: Load .so files, spin up threads, run games
}

#include "Simulator.h"
#include <iostream>
#include <string>
#include <thread>
#include <vector>


int main(int argc, char* argv[]) {
    bool verbose = false;
    size_t num_threads = 1;
    Simulator simulator;
    Config cfg = simulator.parseArgs(argc, argv);
    std::vector<std::string> errors = simulator.varifyParameters(cfg);
    if (!errors.empty()) {
        std::cerr << "Errors found in configuration:" << std::endl;
        for (const auto& error : errors) {
            std::cerr << " - " << error << std::endl;
        }
        return 1; // Exit with error code
    }
    simulator.assignParameters(cfg);
    if(simulator.getRunMode() == RunMode::Comparative) {
        std::string input_map = cfg.args.at("game_map");
        std::string game_managers_folder = cfg.args.at("game_managers_folder");
        std::string player1_algorithm = cfg.args.at("algorithm1");
        std::string player2_algorithm = cfg.args.at("algorithm2");
        simulator.runComparative(input_map, game_managers_folder, player1_algorithm, player2_algorithm);
    } else if (simulator.getRunMode() == RunMode::Competitive) {
        std::string input_maps_floder = cfg.args.at("game_maps_folder");
        std::string game_manager = cfg.args.at("game_manager");
        std::string algorithms_folder = cfg.args.at("algorithms_folder");
        simulator.runCompetitive(input_maps_floder, game_manager, algorithms_folder);
    } else {
        std::cerr << "Invalid run mode specified." << std::endl;
        return 1; // Exit with error code
    }
    return 0;
}
#pragma once
#include <cstddef>
#include <string>
class Simulator {
private:
    bool verbose_;
    size_t num_threads_;
public:
    // Constructor
    Simulator(bool verbose, size_t num_threads)
        : verbose_(verbose), num_threads_(num_threads) {}
    // Method to run the simulation
    void runComparative(
                        const std::string& input_map, 
                        const std::string& game_managers_folder, 
                        const std::string& player1_algorithm, 
                        const std::string& player2_algorithm 
    ) const;

    void runCompetitive(
                       const std::string& input_maps_floder,
                        const std::string& game_manager,
                        const std::string& algoritms_folder
    ) const;

};
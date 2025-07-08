#include "Simulator.h"
#include <iostream>
#include <string>
#include <thread>
#include <vector>

int main(int argc, char* argv[]) {
    bool verbose = false;
    size_t num_threads = 1;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--verbose") {
            verbose = true;
        } else if (arg == "--threads" && i + 1 < argc) {
            num_threads = std::stoul(argv[++i]);
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 1;
        }
    }

    Simulator simulator(verbose, num_threads);
    simulator.runComparative("input_map.txt", "game_managers/", "player1_algorithm.so", "player2_algorithm.so");

    return 0;
}
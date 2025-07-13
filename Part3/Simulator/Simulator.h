#pragma once
#include <cstddef>
#include <string>
#include <iostream>
#include "RunMode.h"
#include <algorithm>
#include <unordered_map>
#include <thread>
#include <vector>
#include <AlgorithmRegistrar.h>
#include <GameManagerRegistrar.h>
#include <../common/GameResult.h>
struct pair_hash {
    std::size_t operator()(const std::pair<int, GameResult::Reason>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(static_cast<int>(p.second)) << 1);
    }
};

struct Config {
    bool verbose = false; // Default to not verbose
    std::unordered_map<std::string, std::string> args; 
    RunMode run_mode = RunMode::Invalid; // Default to invalid mode
};

class Simulator {
private:
    RunMode run_mode = RunMode::Invalid; // Default to invalid mode
    bool verbose_;
    size_t num_threads_ = 1;
    std::unordered_map<std::string, std::string> params;
    std::vector<std::string> getFilesInFolder(const std::string& folder) const;
    bool loadSO(const std::string& file_path) const;
    

public:
    // Constructor
    Simulator();
    // Constructor with config
    explicit Simulator(const Config& cfg);

    // Method to parse command line arguments
    Config parseArgs(int argc, char* argv[]);
    // Method to verify parameters
    std::vector<std::string> varifyParameters(const Config& cfg) const;
    // Method to assign parameters based on the configuration
    void assignParameters(const Config& cfg);
    // Method to run the simulation
    bool runComparative(
                        const std::string& input_map, 
                        const std::string& game_managers_folder, 
                        const std::string& player1_algorithm, 
                        const std::string& player2_algorithm 
    ) const;

    bool runCompetitive(
                       const std::string& input_maps_floder,
                        const std::string& game_manager,
                        const std::string& algoritms_folder
    ) const;

    //setters and getters
    RunMode getRunMode() const { return run_mode; }
    bool isVerbose() const { return verbose_; }
    size_t getNumThreads() const { return num_threads_; }
    void setRunMode(RunMode mode) { run_mode = mode; }
    void setVerbose(bool verbose) { verbose_ = verbose; }
    void setNumThreads(size_t num_threads) { num_threads_ = num_threads; }  
    std::unordered_map<std::string, std::string> getParams() const { return params; }
    void addParam(const std::string& key, const std::string& value) {
        params[key] = value;
    }
};
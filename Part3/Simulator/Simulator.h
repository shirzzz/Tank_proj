#pragma once
#include <cstddef>
#include <string>
#include <iostream>
#include "RunMode.h"
#include <algorithm>
#include <unordered_map>
#include <thread>
#include <vector>
#include "AlgorithmRegistrar.h"
#include "GameManagerRegistrar.h"
#include <../common/GameResult.h>
#include <queue>
#include <mutex>
#include <memory>
struct ComparativeTask {
    std::shared_ptr<AbstractGameManager> game_manager;
    std::string game_map;
    TankAlgorithmFactory algo1_factory;
    TankAlgorithmFactory algo2_factory;
    bool verbose;
    size_t num_threads;
    std::string output_path;
};

struct CompetitiveTask {
    std::shared_ptr<AbstractGameManager> game_manager;
    std::string map_file;
    std::string algo1_name;
    std::string algo2_name;
    TankAlgorithmFactory algo1_factory;
    TankAlgorithmFactory algo2_factory;
    bool verbose;
    size_t num_threads;
};


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

enum class SOType { Algorithm, GameManager };

class Simulator {
private:
    RunMode run_mode = RunMode::Invalid; // Default to invalid mode
    bool verbose_;
    size_t num_threads_ = 1;
    std::unordered_map<std::string, std::string> params;
    std::vector<std::string> getFilesInFolder(const std::string& folder) const;
    bool loadSO(const std::string& file_path, SOType type) const;
    

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

    void runComparativeWorker(std::queue<ComparativeTask>& tasks_queue, 
                                std::mutex& queue_mutex,
                                std::mutex& output_mutex,
                                std::unordered_map<std::pair<int, GameResult>, std::vector<std::string>, pair_hash>& game_results);

    void runCompetitiveWorker(std::queue<CompetitiveTask>& tasks_queue,
                                std::mutex& queue_mutex,
                                std::mutex& score_mutex,
                                std::unordered_map<std::string, int>& game_results);
                                
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
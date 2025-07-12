#include "Simulator.h"
#include <iostream>
#include <thread>
#include <algorithm>
#include <dlfcn.h> // For dynamic loading of shared libraries
#include <filesystem>
#include <GameManagerRegistration.h>
    // Constructor
    Simulator::Simulator(){};
    // Constructor with config
    explicit Simulator::Simulator(const Config& cfg)
        : verbose_(cfg.verbose), num_threads_(std::max<size_t>(1, std::thread::hardware_concurrency())) {
        if (cfg.args.find("num_threads") != cfg.args.end()) {
            num_threads_ = std::stoul(cfg.args.at("num_threads"));
        }
        if (cfg.args.find("run_mode") != cfg.args.end()) {
            std::string mode_str = cfg.args.at("run_mode");
            if (mode_str == "Comparative") {
                run_mode = RunMode::Comparative;
            } else if (mode_str == "Competitive") {
                run_mode = RunMode::Competitive;
            } else {
                run_mode = RunMode::Invalid;
            }
        }
    }

    Config parseArgs(int argc, char* argv[]) {
        Config cfg;
        cfg.verbose = false; // Default to not verbose
        cfg.run_mode = RunMode::Invalid; // Default to invalid mode
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-verbose") {
                cfg.verbose = true;
            } else if (arg == "-comparative") {
                cfg.run_mode = RunMode::Comparative;
            } else if (arg == "-competition") {
                cfg.run_mode = RunMode::Competitive;
            } else if (arg.find("--num_threads=") == 0) {
                cfg.args["num_threads"] = std::stoul(arg.substr(14));
            } else {
                // Handle other arguments
                size_t pos = arg.find('=');
                if (pos != std::string::npos) {
                    std::string key = arg.substr(0, pos);
                    std::string value = arg.substr(pos + 1);
                    cfg.args[key] = value;
                } else {
                    std::cerr << "Unknown argument: " << arg << std::endl;
                }
            } 
        }
        return cfg;
    }

std::vector<std::string> Simulator::varifyParameters(const Config& cfg) const{
    std::vector<std::string> errors;
    if (cfg.run_mode == RunMode::Invalid) {
        std::cerr << "Error: Invalid run mode specified." << std::endl;
        errors.push_back("run_mode");
    }
    else if(cfg.run_mode == RunMode::Comparative){
        if (cfg.args.find("game_map") == cfg.args.end()){
            std::cerr << "Error: Missing required argument 'game_map' for comparative mode." << std::endl;
            errors.push_back("game_map");
        }
        if (cfg.args.find("game_managers_folder") == cfg.args.end()) {
            std::cerr << "Error: Missing required argument 'game_managers_folder' for comparative mode." << std::endl;
            errors.push_back("game_managers_folder");
        }
        if (cfg.args.find("algorithm1") == cfg.args.end()) {
            std::cerr << "Error: Missing required argument 'algorithm1' for comparative mode." << std::endl;
            errors.push_back("algorithm1");
        }
        if (cfg.args.find("algorithm2") == cfg.args.end()) {
            std::cerr << "Error: Missing required argument 'algorithm2' for comparative mode ." << std::endl;
            errors.push_back("algorithm2");
        }
        } else if (cfg.run_mode == RunMode::Competitive) {
            if (cfg.args.find("game_maps_folder") == cfg.args.end()){
                std::cerr << "Error: Missing required argument 'game_maps_folder' for competitive mode." << std::endl;
                errors.push_back("game_maps_folder");
            }
            if (cfg.args.find("game_manager") == cfg.args.end()) {
                std::cerr << "Error: Missing required argument 'game_manager' for competitive mode." << std::endl;
                errors.push_back("game_manager");
            }
            if (cfg.args.find("algorithms_folder") == cfg.args.end()) {
                std::cerr << "Error: Missing required argument 'algorithms_folder' for competitive mode." << std::endl;
                errors.push_back("algorithms_folder");
            }
    }
    return errors;
}

void Simulator::assignParameters(const Config& cfg) {
    if (cfg.args.find("verbose") != cfg.args.end()) {
        verbose_ = cfg.args.at("verbose") == "true";
    }
    if (cfg.args.find("num_threads") != cfg.args.end() && !cfg.args.at("nums_threads").empty() && cfg.args.at("num_threads") >= "1") {
        num_threads_ = std::stoul(cfg.args.at("num_threads")) + 1; // +1 to account for the main thread
    }
    if (cfg.args.find("run_mode") != cfg.args.end()) {
        std::string mode_str = cfg.args.at("run_mode");
        if (mode_str == "Comparative") {
            run_mode = RunMode::Comparative;
        } else if (mode_str == "Competitive") {
            run_mode = RunMode::Competitive;
        } else {
            run_mode = RunMode::Invalid;
        }
    }
    if(run_mode == RunMode::Comparative) {
        if (cfg.args.find("game_map") != cfg.args.end()) {
            std::cout << "Game Map: " << cfg.args.at("game_map") << std::endl;
            addParam("game_map", cfg.args.at("game_map"));
        }
        if (cfg.args.find("game_managers_folder") != cfg.args.end()) {
            std::cout << "Game Managers Folder: " << cfg.args.at("game_managers_folder") << std::endl;
            addParam("game_managers_folder", cfg.args.at("game_managers_folder"));
        }
        if (cfg.args.find("algorithm1") != cfg.args.end()) {
            std::cout << "Algorithm 1: " << cfg.args.at("algorithm1") << std::endl;
            addParam("algorithm1", cfg.args.at("algorithm1"));
        }
        if (cfg.args.find("algorithm2") != cfg.args.end()) {
            std::cout << "Algorithm 2: " << cfg.args.at("algorithm2") << std::endl;
            addParam("algorithm2", cfg.args.at("algorithm2"));
        }
    } else if (run_mode == RunMode::Competitive) {
        if (cfg.args.find("game_maps_folder") != cfg.args.end()) {
            std::cout << "Game Maps Folder: " << cfg.args.at("game_maps_folder") << std::endl;
            addParam("game_maps_folder", cfg.args.at("game_maps_folder"));
        }
        if (cfg.args.find("game_manager") != cfg.args.end()) {
            std::cout << "Game Manager: " << cfg.args.at("game_manager") << std::endl;
            addParam("game_manager", cfg.args.at("game_manager"));
        }
        if (cfg.args.find("algorithms_folder") != cfg.args.end()) {
            std::cout << "Algorithms Folder: " << cfg.args.at("algorithms_folder") << std::endl;
            addParam("algorithms_folder", cfg.args.at("algorithms_folder"));
        }
    }
}

bool Simulator::loadSO(const std::string& file_path) const {
    // Load the shared object file (SO) using dlopen or similar method
    std::string name_so = std::filesystem::path(file_path).stem();
    auto& registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    registrar.createAlgorithmFactoryEntry(name_so);
    dlerror(); // Clear any existing error
    void* handle = dlopen(file_path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error loading shared object file: " << dlerror() << std::endl;
        registrar.removeLast(); // Remove the last entry if loading fails
        return false;
    }
    try{
        registrar.validateLastRegistration();
    } catch (const AlgorithmRegistrar::BadRegistrationException& e) {
        std::cerr << "Bad registration for algorithm: " << e.name << std::endl;
        std::cerr << "Has name: " << e.hasName << ", Has Player Factory: " << e.hasPlayerFactory 
                  << ", Has Tank Algorithm Factory: " << e.hasTankAlgorithmFactory << std::endl;
        registrar.removeLast(); // Remove the last entry if validation fails
        return false;
    }
    std::cout << "Loaded successfully: " << file_path << std::endl;
    return true; // Indicate success
}

std::vector<std::string> Simulator::getFilesInFolder(const std::string& folder) const {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        if (entry.path().extension() == ".so") {
            files.push_back(entry.path().string());
        }
    }
    return files;
}

bool Simulator::runComparative(const std::string& game_map,
                                const std::string& game_managers_folder,
                                const std::string& algo1,
                                const std::string& algo2) const {
                                    if(!loadSO(algo1) || !loadSO(algo2)) {
                                        std::cerr << "Failed to load algorithms." << std::endl;
                                        return false;
                                    }
                                    std::vector<std::string> game_managers = getFilesInFolder(game_managers_folder);
                                    if (game_managers.empty()) {
                                        std::cerr << "No game managers found in folder: " << game_managers_folder << std::endl;
                                        return false;
                                    }
                                    for(const auto& manager : game_managers) {
                                        if(!loadSO(manager)) {
                                            std::cerr << "Failed to load game manager: " << manager << std::endl;
                                            return false;
                                        }
                                    }
                                    auto& registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
                                    if (registrar.count() < 2) {
                                        std::cerr << "Not enough algorithms registered for comparative simulation." << std::endl;
                                        return false;
                                    }
                                    const auto& algo1_factory = registrar.begin()->createTankAlgorithm(0, 0);
                                    const auto& algo2_factory = registrar.begin()->createTankAlgorithm(1, 0);
                                    if (!algo1_factory || !algo2_factory) {
                                        std::cerr << "Failed to create tank algorithms." << std::endl;
                                        return false;
                                    }
                                    auto& manager_registrar = GameManagerRegistrar::getGameManagerRegistrar();
                                    if (manager_registrar.empty()) {
                                        std::cerr << "No game managers registered." << std::endl;
                                        return false;
                                    }
                                    auto& game_manager = manager_registrar.begin()->createGameManager();
                                    //Itai for god sake I dont know what to do with this, I just added it

    std::cout << "Running comparative simulation..." << std::endl;
    return true; // Indicate success
} 

bool Simulator::runCompetitive(const std::string& maps_folder,
                                const std::string& game_manager,
                                const std::string& algorithms_folder) const {
    std::vector<std::string> algorithms = getFilesInFolder(algorithms_folder);
    if (algorithms.empty()) {
        std::cerr << "No algorithms found in folder: " << algorithms_folder << std::endl;
        return false;
    }
    for (const auto& algo : algorithms) {
        if (!loadSO(algo)) {
            std::cerr << "Failed to load algorithm: " << algo << std::endl;
            return false;
        }
    }
    if (!loadSO(game_manager)) {
        std::cerr << "Failed to load game manager: " << game_manager << std::endl;
        return false;
    }
    std::vector<std::string> maps = getFilesInFolder(maps_folder);
    if (maps.empty()) {
        std::cerr << "No maps found in folder: " << maps_folder << std::endl;
        return false;
    }
    for (int k = 0; k < maps.size(); ++k) {
        for(int i = 0; i < algorithms.size(); ++i) {
            std::cout << "Running game with map: " << maps[k] 
                        << ", algorithm1: " << algorithms[i] 
                        << ", algorithm2: " << algorithms[(i+1+k%(algorithms.size() - 1))%(algorithms.size())] << std::endl;

        }
    }
}

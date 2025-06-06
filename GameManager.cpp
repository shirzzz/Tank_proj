#include "GameManager.h"
#include "GameBoard.h"
#include "Tank.h"
#include "Shell.h"
#include "DestructionCause.h"
#include "Chased.h"
#include "DirectionUtils.h"
#include "common/ActionRequest.h"
#include "CanonDirection.h"
#include "Wall.h"
#include "MySatelliteView.h"
#include "CellType.h"
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include "common/Player1.h"
#include "common/Player2.h"
#include <fstream>
#include "MyBattleInfo.h"
#include "Mine.h"

Player1* player1 = nullptr;
Player2* player2 = nullptr;


void GameManager::updateShells() const {
    auto& shells = shared_board->getShells();
    for (Shell& shell : shells) {
        shell.storePreviousPosition();
    }
    for (Shell& shell : shells) {
        shared_board->moveShell(&shell);
    }
}

void GameManager::resolveShellCollisions() {
    auto& shells = shared_board->getShells();
    std::vector<Shape*> toExplode;

    // Shell vs Shell collisions
    for (size_t i = 0; i < shells.size(); ++i) {
        for (size_t j = i + 1; j < shells.size(); ++j) {
            auto pi = shells[i].getPosition();
            auto pj = shells[j].getPosition();
            auto oi = shells[i].getPreviousPosition();
            auto oj = shells[j].getPreviousPosition();
            if (pi == pj || (pi == oj && pj == oi)) {
                toExplode.push_back(&shells[i]);
                toExplode.push_back(&shells[j]);
            }
        }
    }

    // Shell vs Wall or Tank
    for (Shell& shell : shells) {
        auto [x, y] = shell.getPosition();
        std::shared_ptr<Shape> cell = shared_board->getCell(x, y);

        if (cell->getCellType() == CellType::WALL) {
            Wall* wall = static_cast<Wall*>(cell.get());
            wall->setLives(wall->getLives() - 1);
            if (wall->getLives() <= 0) {
                shared_board->removeWall(wall);
                toExplode.push_back(wall);
            }
            toExplode.push_back(&shell);
        } else if (cell->getCellType() == CellType::TANK1 || cell->getCellType() == CellType::TANK2) {
            std::shared_ptr<Tank> hit_tank = std::dynamic_pointer_cast<Tank>((shared_board->getCell(x, y)));
            if (hit_tank) {
                hit_tank->killTank();
                if(hit_tank->getIndexTank() == '1') {
                    player1.addKilledTank(hit_tank.get()->getIndexTank());
                    player1.setNumKilledTanks(player1.getNumKilledTanks() + 1);
                    player1.removeTank(hit_tank);
                } else if (hit_tank->getIndexTank() == '2') {
                    player2.addKilledTank(hit_tank.get()->getIndexTank());
                    player2.setNumKilledTanks(player2.getNumKilledTanks() + 1);
                    player2.removeTank(hit_tank);
                }     
                shared_board->removeTank(hit_tank);
                toExplode.push_back(&shell);
            }
        }
    }

    for (auto* shape : toExplode) {
        switch (shape->getCellType()) {
            case CellType::WALL:
                shared_board->removeWall(static_cast<Wall*>(shape));
                break;
            case CellType::SHELL:
                shared_board->removeShell(*static_cast<Shell*>(shape));
                break;
            default:
                break;
        }
    }
}

void GameManager::resolveTankCollisions() {
    auto player1Tanks = player1.getTanks();
    auto player2Tanks = player2.getTanks();

    for (auto& tank1 : player1Tanks) {
        for (auto& tank2 : player2Tanks) {
            if (!tank1 || !tank2) continue;
            auto p1 = tank1->getPosition();
            auto p2 = tank2->getPosition();
            auto o1 = tank1->getPreviousPosition();
            auto o2 = tank2->getPreviousPosition();

            if (p1 == p2 || (p1 == o2 && p2 == o1)) {
                player1.addKilledTank(tank1->getIndexTank());
                player1.setNumKilledTanks(player1.getNumKilledTanks() + 1);
                shared_board->removeTank(tank1);
                player1.removeTank(tank1);
                tank1->killTank();
                tank2->killTank();
                player2.addKilledTank(tank2->getIndexTank());
                player2.setNumKilledTanks(player2.getNumKilledTanks() + 1);
                shared_board->removeTank(tank2);
                player2.removeTank(tank2);
            }
        }
    }
    std::vector<std::shared_ptr<Tank>> all_tanks;
    all_tanks.reserve(player1.getTanks().size() + player2.getTanks().size());
    all_tanks.insert(all_tanks.end(), player1.getTanks().begin(), player1.getTanks().end());
    for (auto& tank : all_tanks) {
        if (!tank) continue;
        auto [x, y] = tank->getPosition();
        if (shared_board->getCell(x, y)->getCellType() == CellType::MINE) {
            if(tank->getIndexTank() == '1') {
                player1.addKilledTank(tank->getIndexTank());
                player1.setNumKilledTanks(player1.getNumKilledTanks() + 1);
                player1.removeTank(tank);
            } else if (tank->getIndexTank() == '2') {
                player2.addKilledTank(tank->getIndexTank());
                player2.setNumKilledTanks(player2.getNumKilledTanks() + 1);
                player2.removeTank(tank);
            }
            tank->killTank();
            shared_board->removeTank(tank);
            shared_board->setCell(x, y, std::make_shared<Empty>(x, y));
        }
    }

    auto& shells = shared_board->getShells();
    for (const Shell& shell : shells) {
        auto [sx, sy] = shell.getPosition();
        for (auto& tank : all_tanks) {
            if (tank && tank->getPosition() == std::make_pair(sx, sy)) {
                // If the tank is hit by a shell
                tank->killTank();
                if(tank->getIndexTank() == '1') {
                    player1.addKilledTank(tank->getIndexTank());
                    player1.setNumKilledTanks(player1.getNumKilledTanks() + 1);
                    player1.removeTank(tank);
                } else if (tank->getIndexTank() == '2') {
                    player2.addKilledTank(tank->getIndexTank());
                    player2.setNumKilledTanks(player2.getNumKilledTanks() + 1);
                    player2.removeTank(tank);
                }
                shared_board->removeTank(tank);
                shared_board->removeShellAtfromBoard(sx, sy);
                break;
            }
        }
    }

    endGame();
}



void GameManager::processAction(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm, ActionRequest action) {
    int waiting_to_go_back = tank->getWaitingToGoBack();
    int waiting_to_shoot = tank->getWaitingToShootAgain();
    
    
    if (waiting_to_go_back == 0) {
        std::pair<int, int> new_position = tank->moveBackward(shared_board->getWidth(), shared_board->getHeight());
        if(shared_board->isCellWalkable(new_position.first, new_position.second)){
            tank->setWaitingToGoBack(-1);
            if(tank->isAlive()){
                tank->addAction("MoveBackward");
                shared_board->moveTank(tank, new_position.first, new_position.second);
            }
            else {
                tank->addAction("MoveBackward (killed)");
            }
        }
        else if(shared_board->isSteppingWall(new_position.first, new_position.second)){
            tank->addAction("MoveBackward (Ignored)");
        }
    }
        

    switch (action) {
        case ActionRequest::GetBattleInfo:{
            MySatelliteView view(*shared_board);
            MyBattleInfo info(tank->getX(), tank->getY(), tank->getCanonDirection(), view);
            if(tank->getIndexTank() == '1') {
                player1.updateTankWithBattleInfo(tank_algorithm, view);
            } else if (tank->getIndexTank() == '2') {
                player2.updateTankWithBattleInfo(tank_algorithm, view);
            }
            if(tank->isAlive())
                tank->addAction("GetBattleInfo");
            else
                tank->addAction("GetBattleInfo (killed)");
            break;
        }
        case ActionRequest::MoveForward:
            if (waiting_to_go_back >= 1){
                tank->setWaitingToGoBack(-1);
                tank->addAction("MoveForward (Ignored)");
            }
            else {
                std::pair<int, int> new_position = tank->moveForward(shared_board->getWidth(), shared_board->getHeight());
                if(shared_board->isCellWalkable(new_position.first, new_position.second)){
                    if(tank->isAlive()) {
                        tank->setWaitingToGoBack(-1);
                        tank->addAction("MoveForward");
                        shared_board->moveTank(tank, new_position.first, new_position.second);
                    } else {
                        tank->addAction("MoveForward (killed)");
                    }    
                }
                else if(shared_board->isSteppingWall(new_position.first, new_position.second)){
                    tank->addAction("MoveForward (Ignored)");
                }
            }
            break;

        case ActionRequest::MoveBackward:
            if (waiting_to_go_back >= 1) {
                tank->addAction("MoveBackward (Ignored)");
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            } 
            break;

        case ActionRequest::RotateLeft45:
            if (waiting_to_go_back == -1) {
                if(tank->isAlive()){
                    tank->addAction("RotateLeft45");
                    tank->rotateEighthLeft();
                }
                else
                    tank->addAction("RotateLeft45 (killed)");
            }
            else {
                tank->addAction("RotateLeft45 (ignored)");
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            }
            break;
        case ActionRequest::RotateRight45:
            if (waiting_to_go_back == -1) {
                if(tank->isAlive()){
                    tank->addAction("RotateRight45");
                    tank->rotateEighthRight();
                }
                else
                    tank->addAction("RotateRight45 (killed)");
            }
            else {
                tank->addAction("RotateRight45 (ignored)");
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            }
            break;
        case ActionRequest::RotateLeft90:
            if (waiting_to_go_back == -1) {
                if(tank->isAlive()){
                    tank->addAction("RotateLeft90");
                    tank->rotateQuarterLeft();
                }
                else
                    tank->addAction("RotateLeft90 (killed)");
            }
            else {
                tank->addAction("RotateLeft90 (ignored)");
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            }
            break;
        case ActionRequest::RotateRight90:
            if (waiting_to_go_back == -1) {
                if(tank->isAlive()){
                    tank->addAction("RotateRight90");
                    tank->rotateQuarterRight();
                }
                else
                    tank->addAction("RotateRight90 (killed)");
            }
            else {
                tank->addAction("RotateRight90 (ignored)");
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            }
            break;

            case ActionRequest::Shoot:
            if (waiting_to_shoot != -1) {
                if(tank->isAlive()){
                    tank->addAction("Shoot (ignored)");
                    tank->setWaitingToShootAgain(waiting_to_shoot - 1);
                }
                else
                    tank->addAction("Shoot (killed)");
                
            } else if (tank->getNumBullets() == 0) {
                if(tank->isAlive())
                    tank->addAction("Shoot (ignored)");
                else
                    tank->addAction("Shoot (killed)");

                //Shir: I think we need to handle the case where all the tanks are out of ammo
                if(tank->getIndexTank() == '1'){
                    player1.player1Shoot();

                }
                else if(tank->getIndexTank() == '2'){
                    player2.player2Shoot();
                }
                if(player1.getSumShells() == 0 && player2.getSumShells() == 0) {
                    // If both players are out of shells, end the game
                    moves_left = 40; // Set moves_left to 40 to end the game
                }
            } else {
                tank->shoot();
        
                // Calculate shell spawn position one step ahead
                int dx = 0, dy = 0;
                std::tie(dx, dy) = directionToVector(tank->getCanonDirection());
        
                int shell_x = (tank->getX() + dx + shared_board->getWidth()) % shared_board->getWidth();
                int shell_y = (tank->getY() + dy + shared_board->getHeight()) % shared_board->getHeight();
        
                // Place shell one step ahead
                shared_board->addShell(*(std::make_shared<Shell>(shell_x, shell_y, tank->getCanonDirection())));

                tank->addAction("Shoot");
                tank->setWaitingToShootAgain(4);
            }
            break;
        case ActionRequest::DoNothing:
            if (waiting_to_go_back == -1) {
                if(tank->isAlive()){
                    tank->addAction("DoNothing");
                }
                else
                    tank->addAction("DoNothing (killed)");
            } else {
                tank->addAction("DoNothing (ignored)");
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            }
            break;
    }
}

void GameManager::updateGame() {
    if (game_over) return;
    for(auto& tank_algorithm : player1.getTankAlgorithms()) {
        std::shared_ptr<Tank> tank = tank_algorithm->getTank();
        if (tank) {
            tank->setPreviousPosition();
            ActionRequest action1 = tank_algorithm->getAction();
            processAction(tank, *tank_algorithm, action1);
        }
    }
    for(auto& tank_algorithm : player2.getTankAlgorithms()) {
        std::shared_ptr<Tank> tank = tank_algorithm->getTank();
        if (tank) {
            tank->setPreviousPosition();
            ActionRequest action2 = tank_algorithm->getAction();
            processAction(tank, *tank_algorithm, action2);
        }
    }
}
// //Shir: why do we need this?
// void GameManager::removeTank(char index) {
//     if (index == '1') {
//         shared_board->removeTank(tank1);
//         lastKnownTank1 = nullptr;
//     } else if (index == '2') {
//         shared_board->removeTank(tank2);
//         lastKnownTank2 = nullptr;
//     }
// }

void GameManager::run() {
    int step = 0;
    std::ofstream file("Output.txt");
    if (!file) {
        std::cerr << "Failed to open Output file for writing." << std::endl;
        return;
    }
    if(player1.getNumTanks() == 0 && player2.getNumTanks() == 0){
        std::cout << "Tie, both players have zero tanks " << std::endl;
        file << "Tie, both players have zero tanks" << std::endl;
        wining_tank = '0';
        endGame();
        return;
    }

    else if(player1.getNumTanks() == 0){
        wining_tank = '2';
        std::cout << "Game Over: Player 1 has no tanks left!" << std::endl;
        file << "Player 2 won with " <<player2.getTanks().size()<<"tanks still alive"<<std::endl;
        endGame();
        return;
    }
    else if(player2.getNumTanks() == 0){
        wining_tank = '1';
        std::cout << "Game Over: Player 2 has no tanks left!" << std::endl;
        file<< "Player 1 won with " <<player1.getTanks().size()<<"tanks still alive"<<std::endl;
        endGame();
        return;
    }
    while (!isGameOver() && moves_left > 0) {
        updateShells();
        resolveShellCollisions();

        if (step % 2 == 0) {
            updateGame();
            resolveTankCollisions();
        }
        //game_board.displayBoard(); 
        step++;
        moves_left = moves_left - 1;
        if (moves_left == 0) {
            
            std::cout << "Game Over: No moves left!" << std::endl;
            wining_tank = '0';
            endGame();
            // *** need to change this part *** Shir: I dont think we need to set the destruction cause here
            // getTank1()->setDestructionCause(DestructionCause::OUTOFAMMO);
            // getTank2()->setDestructionCause(DestructionCause::OUTOFAMMO);
            }
        }
    
    for(auto& tank : player1.getTanks()) {
        for(size_t j = 0; j < tank->getActions().size() - 1; j++) {
            file << tank->getActions()[j] << ",";
        }
        file << tank->getActions().back() << " ";
        file << std::endl;
    }
    for(auto& tank : player2.getTanks()) {
       for(size_t j = 0; j < tank->getActions().size() -1; j++) {
            file << tank->getActions()[j] << ",";
    }
        file << tank->getActions().back() << " ";
        file << std::endl;
    }

    if(wining_tank == '1'){
        file << "Player 1 won with " <<player1.getNumTanks() - player1.getNumKilledTanks()<< "still alive"<< std::endl;
    }
    else if(wining_tank == '2'){
        file << "Player 2 won with " <<player2.getNumTanks() - player2.getNumKilledTanks()<< "still alive"<< std::endl;
    }
    else if(wining_tank == '0'){
        file << "Tie, reached max steps = " << max_steps<<", player 1 has "<<player1.getNumTanks() - player1.getNumKilledTanks()<<
        ", player2 has "<<player2.getNumTanks() - player2.getNumKilledTanks()<< std::endl;
    }
    else{
        file << "Game Over: No moves left!" << std::endl;
    }

    file.close();
    std::cout << "Finished writing to Output.txt successfully." << std::endl;
    }

    bool GameManager::isGameEnded() {
        if(player1.getTanks().size() == 0 && player2.getTanks().size() == 0){
            std::cout << "Game Over: Both players have no tanks left!" << std::endl;
            wining_tank = '0';
            return true;
        }
        else if(player1.getTanks().size() == 0){
            std::cout << "Game Over: Player 1 has no tanks left!" << std::endl;
            wining_tank = '2';
            return true;
        }
        else if(player2.getTanks().size() == 0){
            std::cout << "Game Over: Player 2 has no tanks left!" << std::endl;
            wining_tank = '1';
            return true;
        }
        else if(moves_left == 0){
            std::cout << "Game Over: No moves left!" << std::endl;
            wining_tank = '0';
            return true;
        }
        return false;
    }

    void GameManager::endGame() {
    displayGame();
    setGameOver(true);
}

void GameManager::readBoard(const std::string& filename) {
        std::ifstream file_board(filename);
        if (!file_board.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }
        if (!loadBoardFromFile(file_board, filename)) {
        std::cerr << "Error loading board from file." << std::endl;
        return;
        }
        file_board.close();
    }

 bool GameManager::loadBoardFromFile(std::istream& file_board, std::string filename) {
    
    int count_tanks_for_player1 = 0; //counts how many tanks player1 has
    int count_tanks_for_player2 = 0; //counts how many tanks player2 has
    
    // Use filename in error file creation for better traceability
    std::ofstream file_errors("input_errors.txt");
    std::cout << "Opening file for writing input errors from: " << filename << std::endl;
    
    if (!file_errors) {
        file_errors << "Error: Failed to open file for writing input errors from " << filename << ".\n" << std::endl;
        file_errors.close();
        std::cerr << "Failed to open file for writing input errors from " << filename << std::endl;
        return false;
    }
    
    // Log the source file being processed
    file_errors << "Processing board file: " << filename << std::endl;
    file_errors << "===========================================" << std::endl;
    
    std::regex pattern_max_steps(R"(MaxSteps\s*:\s*(\d+))");
    std::regex pattern_num_shells(R"(NumShells\s*:\s*(\d+))");
    std::regex pattern_rows(R"(Rows\s*:\s*(\d+))");
    std::regex pattern_cols(R"(Cols\s*:\s*(\d+))");
    
  
    // Parse configuration parameters (first 4 lines)
    for(int i = 0; i < 4; i++){
        std::string line;
        std::getline(file_board, line);
        if (line.empty()) {
            continue; // Skip empty lines
        }
        std::smatch match;
        if (std::regex_search(line, match, pattern_max_steps)) {
            max_steps = std::stoi(match[1]);
            std::cout << "MaxSteps: " << max_steps << std::endl;
        } else if (std::regex_search(line, match, pattern_num_shells)) {
            num_shells = std::stoi(match[1]);
            std::cout << "NumShells: " << num_shells << std::endl;
        } else if (std::regex_search(line, match, pattern_rows)) {
            height = std::stoi(match[1]);
            std::cout << "Rows: " << height << std::endl;
        } else if (std::regex_search(line, match, pattern_cols)) {
            width = std::stoi(match[1]);
            std::cout << "Cols: " << width << std::endl;
        }
        else {
            // Enhanced error reporting with filename context
            file_errors << "Error in " << filename << " (line " << (i+1) << "): Invalid line format: " << line << std::endl;
            std::cerr << "Invalid line format in " << filename << " (line " << (i+1) << "): " << line << std::endl;
            return false;
        }
    }

      // Initialize players with default values
    player1 = player_factory->create(1, width, height, max_steps, num_shells, 0);
    player2 = player_factory->create(2, width, height, max_steps, num_shells, 0);
    moves_left = max_steps; // Initialize moves left to max steps
    shared_board = std::make_shared<GameBoard>(width, height);

    
    if (width <= 0 || height <= 0) {
        file_errors << "Error in " << filename << ": Invalid board dimensions (width: " << width << ", height: " << height << ")" << std::endl;
        std::cerr << "Invalid board dimensions in " << filename << std::endl;
        return false;
    }
    board = std::vector<std::vector<std::shared_ptr<Shape>>>(height, std::vector<std::shared_ptr<Shape>>(width, nullptr));
    
    // Parse board layout
    for(size_t i = 0; i < height; i++){
        std::string line;
        std::getline(file_board, line);
        if(file_board.eof()){
            line = std::string(width, ' '); // Fill the rest of the board with empty spaces
        }
        if (line.empty()) {
            continue; // Skip empty lines
        }
        
        for(size_t j = 0; j < width; j++){
            char c = ' ';
        
            if(j < static_cast<int>(line.size())){
                c = line[j];
            }
            
            if (c == '1') {
                count_tanks_for_player1++;
                std::shared_ptr<Tank> current_tank = std::make_shared<Tank>(j, i, '1');
                board[i][j] = current_tank;
                player1.addTank(current_tank);
                player1.addTankAlgorithm(tank_factory->create(1, count_tanks_for_player1 - 1)); // Add tank algorithm for player 1
            }
            else if( c== '2') {
                count_tanks_for_player2++;
                std::shared_ptr<Tank> current_tank = std::make_shared<Tank>(j, i, '2');
                board[i][j] = current_tank;
                player2.addTank(current_tank);
                player2.addTankAlgorithm(tank_factory->create(2, count_tanks_for_player2 - 1)); // Add tank algorithm for player 2
            }
            else if (c == ' ') {
                board[i][j] = std::make_shared<Empty>(j, i);
            }
            else if (c == '#') {
                board[i][j] = std::make_shared<Wall>(j, i);
            }
            else if (c == '@') {
                board[i][j] = std::make_shared<Mine>(j, i);
            }
            else {
                // Enhanced error reporting with filename and position context
                board[i][j] = std::make_shared<Empty>(j, i); // Default to empty cell
            }
        }
    }    
    GameBoard game_board(width, height, board);
    shared_board = std::make_shared<GameBoard>(game_board); // Initialize shared_board with the game board
    shared_board->displayBoard(); // Display the loaded board
    player1.setNumTanks(count_tanks_for_player1);
    player2.setNumTanks(count_tanks_for_player2);
    
    // Log successful completion
    file_errors << "Successfully loaded board from " << filename << std::endl;
    file_errors << "Player 1 tanks: " << count_tanks_for_player1 << std::endl;
    file_errors << "Player 2 tanks: " << count_tanks_for_player2 << std::endl;
    
    file_errors.close();
    std::cout << "Board loaded successfully from " << filename << std::endl;
    return true;
}


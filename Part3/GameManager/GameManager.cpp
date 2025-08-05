#include "GameManager.h"
#include "../Algorithm/GameBoard.h"
#include "../UserCommon/Tank.h"
#include "../UserCommon/Shell.h"
#include "../DestructionCause.h"
#include "../Chased.h"
#include "../UserCommon/DirectionUtils.h"
#include "../common/ActionRequest.h"
#include "../UserCommon/CanonDirection.h"
#include "../UserCommon/Wall.h"
#include "../UserCommon/MySatelliteView.h"
#include "../UserCommon/CellType.h"
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include "../Algorithm/Player1.h"
#include "../Algorithm/Player2.h"
#include <fstream>
#include "../UserCommon/MyBattleInfo.h"  // ADDED: Include your concrete battle info
#include "../UserCommon/Mine.h"
#include "../UserCommon/Empty.h"
#include "../UserCommon/MyPlayerFactory.h"  // ADDED: Include your concrete factories
#include "../UserCommon/MyTankAlgorithmFactory.h"  // ADDED: Include your concrete factories
#include <filesystem> // C++17

namespace GameManager_211466123_212399455 {

void GameManager::updateShells() const {
    auto& shells = shared_board->getShells();
    for (Shell& shell : shells) {
        shell.storePreviousPosition();
    }
    for (Shell& shell : shells) {
        shared_board->moveShell(&shell);
    }
}

void GameManager::updateTanks() const{
    auto player1Tanks = this->player1.getTanks();
    auto player2Tanks = this->player2.getTanks();
    
    for (auto& tank : player1Tanks) {
        if (tank && tank->isAlive()) {
            shared_board->moveTank(tank, tank->getNextPosition().first, tank->getNextPosition().second);
        }
    }

    for (auto& tank : player2Tanks) {
        if (tank && tank->isAlive()) {
            shared_board->moveTank(tank, tank->getNextPosition().first, tank->getNextPosition().second);
        }
    }
}

void GameManager::resolveShellCollisions() {
    auto& shells = shared_board->getShells();
    std::vector<Shape*> toExplode;

    detectShellVsShellCollisions(shells, toExplode);
    detectShellVsWallOrTank(shells, toExplode);
    processExplosions(toExplode);
}


void GameManager::detectShellVsShellCollisions(const std::vector<Shell>& shells, std::vector<Shape*>& toExplode) {
    for (size_t i = 0; i < shells.size(); ++i) {
        for (size_t j = i + 1; j < shells.size(); ++j) {
            auto pi = shared_board->getNextPosition(shells[i].getX(), shells[i].getY(), shells[i].getDirection());
            auto pj = shared_board->getNextPosition(shells[j].getX(), shells[j].getY(), shells[j].getDirection());
            auto oi = shells[i].getPosition();
            auto oj = shells[j].getPosition();
            if (pi == pj || (pi == oj && pj == oi)) {
                toExplode.push_back(const_cast<Shell*>(&shells[i]));
                toExplode.push_back(const_cast<Shell*>(&shells[j]));
            }
        }
    }
}


void GameManager::detectShellVsWallOrTank(std::vector<Shell>& shells, std::vector<Shape*>& toExplode) {
    for (Shell& shell : shells) {
        std::pair<int, int> next_position = shared_board->getNextPosition(shell.getX(), shell.getY(), shell.getDirection());
        auto [x, y] = next_position;
        std::shared_ptr<Shape> cell = shared_board->getCell(x, y);

        if (cell->getCellType() == CellType::WALL) {
            Wall* wall = static_cast<Wall*>(cell.get());
            wall->setLives(wall->getLives() - 1);
            if (wall->getLives() <= 0) {
                toExplode.push_back(wall);
            }
            toExplode.push_back(&shell);
        } else if (cell->getCellType() == CellType::TANK1 || cell->getCellType() == CellType::TANK2) {
            handleShellTankCollisions(next_position, shell, toExplode);
        }
    }
}

void GameManager::handleShellTankCollisions(std::pair<int, int> next_position, Shell& shell, std::vector<Shape*>& toExplode) {
    for(auto tank : player1.getTanks()){
        if(tank.get()->getX() == next_position.first && tank.get()->getY() == next_position.second){
             if (tank && tank->isAlive()) {
                tank->killTank();
                shared_board->removeTank(tank);
                toExplode.push_back(&shell);
                return;
            }   
        }
    }
    for(auto tank : player2.getTanks()){
        if(tank.get()->getX() == next_position.first && tank.get()->getY() == next_position.second){
             if (tank && tank->isAlive()) {
                tank->killTank();
                shared_board->removeTank(tank);
                toExplode.push_back(&shell);
                return;
            }   
        }
    }


   
}


void GameManager::processExplosions(std::vector<Shape*>& toExplode) {
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
    std::vector<std::shared_ptr<Tank>> all_tanks = collectAllTanks();
    handleTankToTankCollisions(all_tanks);
    handleTankMineCollisions(all_tanks);
    handleTankShellCollisions(all_tanks);
}


std::vector<std::shared_ptr<Tank>> GameManager::collectAllTanks() {
    std::vector<std::shared_ptr<Tank>> all_tanks;
    all_tanks.reserve(this->player1.getTanks().size() + this->player2.getTanks().size());
    all_tanks.insert(all_tanks.end(), this->player1.getTanks().begin(), this->player1.getTanks().end());
    all_tanks.insert(all_tanks.end(), this->player2.getTanks().begin(), this->player2.getTanks().end());
    return all_tanks;
}

//// Handle collisions between tanks
void GameManager::handleTankToTankCollisions(std::vector<std::shared_ptr<Tank>>& all_tanks) {
    for (auto& tank1 : all_tanks) {
        for (auto& tank2 : all_tanks) {
            if (!tank1 || !tank2 || tank1 == tank2 || !tank1->isAlive() || !tank2->isAlive()) continue;

            auto p1 = tank1->getNextPosition(), p2 = tank2->getNextPosition();
            auto o1 = tank1->getPosition(), o2 = tank2->getPosition();

            if (p1 == p2 || (p1 == o2 && p2 == o1)) {
                for (auto& tank : {tank1, tank2}) {
                    if (tank->isAlive()) {
                        tank->killTank();
                        shared_board->removeTank(tank);
                    }
                }
            }
        }
    }
}

// Handle collisions between tanks and mines
void GameManager::handleTankMineCollisions(std::vector<std::shared_ptr<Tank>>& all_tanks) {
    for (auto& tank : all_tanks) {
        if (!tank) continue;
        auto [x, y] = tank->getNextPosition();
        if (is_mine[y][x]) {
            if (tank->isAlive()) {
                tank->killTank();
                shared_board->removeTank(tank);
            }

            is_mine[y][x] = false;
            shared_board->bombMine(x, y);
        }
    }
}

// Handle collisions between tanks and shells
void GameManager::handleTankShellCollisions(std::vector<std::shared_ptr<Tank>>& all_tanks) {
    auto& shells = shared_board->getShells();
    for (Shell& shell : shells) {
        auto [sx, sy] = shell.getPosition();
        for (auto& tank : all_tanks) {
            if (tank && tank->getPosition() == std::make_pair(sx, sy) && tank->isAlive()) {
                tank->killTank();
                shared_board->removeTank(tank);
                shared_board->removeShell(shell);
                break;
            }
        }
    }
}

// Process the action for a tank based on the action request
void GameManager::processAction(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm, ActionRequest action) {
    int waiting_to_go_back = tank->getWaitingToGoBack();
    logActionStart(tank, tank_algorithm, action);

    // Handle passive backward delay logic before switch
    if (waiting_to_go_back == 0) {
        handlePassiveBackward(tank, tank_algorithm);
    }

    // Main action switch
    switch (action) {
        case ActionRequest::GetBattleInfo:
            handleGetBattleInfo(tank, tank_algorithm);
            break;
        case ActionRequest::MoveForward:
            handleMoveForward(tank, tank_algorithm);
            break;
        case ActionRequest::MoveBackward:
            handleMoveBackward(tank);
            break;
        case ActionRequest::RotateLeft45:
        case ActionRequest::RotateRight45:
        case ActionRequest::RotateLeft90:
        case ActionRequest::RotateRight90:
            handleRotate(tank, tank_algorithm, action);
            break;
        case ActionRequest::Shoot:
            handleShoot(tank, tank_algorithm);
            break;
        case ActionRequest::DoNothing:
            handleDoNothing(tank, tank_algorithm);
            break;
    }
}

// Log the start of an action for a tank
void GameManager::logActionStart(const std::shared_ptr<Tank>& tank, const TankAlgorithm& tank_algorithm, ActionRequest action) {
    std::cout << "-------------------------------------------------------------------------------------------------\n";
    std::cout << "Processing action: " << action
              << " for player: " << tank->getIndexTank() << (tank->isAlive() ? " (alive)" : " (killed)")
              << " for tank index:" << tank_algorithm.getIndexTank()
              << " at position: (" << tank->getX() << ", " << tank->getY() << ")"
              << " in direction: " << tank->getCanonDirection()
              << " algorithm is: " << (tank_algorithm.getPlayerIndex() == 1 ? "BFS" : "CHASED")
              << "\n";
    std::cout << "-------------------------------------------------------------------------------------------------\n";
}

// Handle the case where a tank 
void GameManager::handlePassiveBackward(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm) {
    auto new_position = tank->moveBackward(shared_board->getWidth(), shared_board->getHeight());
    if (!shared_board->isSteppingWall(new_position.first, new_position.second)) {
        if (tank->isAlive()) {
            tank->setWaitingToGoBack(-1);
            tank->addAction("MoveBackward");
            tank->setNextPosition(tank->getX(), tank->getY(), tank->getCanonDirection(), -1,
                                  shared_board->getWidth(), shared_board->getHeight());
            tank_algorithm.setHaveBattleInfo(false);
        } else {
            tank->addAction("MoveBackward (killed)");
        }
    } else {
        tank->addAction("MoveBackward (Ignored)");
    }
}

void GameManager::handleGetBattleInfo(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm) {
    MySatelliteView view(*shared_board);
    MyBattleInfo info(&view, tank->getIndexTank());
    if (tank->isAlive()) {
        if (tank->getIndexTank() == '1') {
            player1.updateTankWithBattleInfo(tank_algorithm, view);
        } else if (tank->getIndexTank() == '2') {
            player2.updateTankWithBattleInfo(tank_algorithm, view);
        }
        tank->addAction("GetBattleInfo");
    } else {
        tank->addAction("GetBattleInfo (killed)");
    }
    tank->setNextPosition(tank->getX(), tank->getY(), tank->getCanonDirection(), 0,
                          shared_board->getWidth(), shared_board->getHeight());
}

void GameManager::handleMoveForward(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm) {
    if (!tank->isAlive()) {
        tank->addAction("MoveForward (killed)");
        return;
    }
    if (tank->getWaitingToGoBack() >= 1) {
        tank->setWaitingToGoBack(-1);
        tank->addAction("MoveForward (Ignored)");
        return;
    }

    auto new_position = tank->moveForward(shared_board->getWidth(), shared_board->getHeight());
    if (!shared_board->isSteppingWall(new_position.first, new_position.second)) {
        tank->setWaitingToGoBack(-1);
        tank->addAction("MoveForward");
        tank->setNextPosition(new_position.first, new_position.second, tank->getCanonDirection(), 1,
                              shared_board->getWidth(), shared_board->getHeight());
        tank_algorithm.setHaveBattleInfo(false);
    } else {
        tank->addAction("MoveForward (Ignored)");
    }
}

void GameManager::handleMoveBackward(std::shared_ptr<Tank> tank) {
    if (!tank->isAlive()) {
        tank->addAction("MoveBackward (killed)");
        return;
    }

    int w = tank->getWaitingToGoBack();
    if (w >= 1) {
        tank->addAction("MoveBackward (Ignored)");
        tank->setWaitingToGoBack(w - 1);
    }
}

void GameManager::handleRotate(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm, ActionRequest action) {
    if (tank->getWaitingToGoBack() != -1) {
        tank->addAction(actionToString(action) + " (ignored)");
        tank->setWaitingToGoBack(tank->getWaitingToGoBack() - 1);
        return;
    }

    if (!tank->isAlive()) {
        tank->addAction(actionToString(action) + " (killed)");
        return;
    }

    tank->addAction(actionToString(action));
    switch (action) {
        case ActionRequest::RotateLeft45: tank->rotateEighthLeft(); break;
        case ActionRequest::RotateRight45: tank->rotateEighthRight(); break;
        case ActionRequest::RotateLeft90: tank->rotateQuarterLeft(); break;
        case ActionRequest::RotateRight90: tank->rotateQuarterRight(); break;
        default: break;
    }
    tank_algorithm.setHaveBattleInfo(false);
    tank->setNextPosition(tank->getX(), tank->getY(), tank->getCanonDirection(), 0,
                          shared_board->getWidth(), shared_board->getHeight());
}

std::string GameManager::actionToString(ActionRequest action) {
    switch (action) {
        case ActionRequest::RotateLeft45: return "RotateLeft45";
        case ActionRequest::RotateRight45: return "RotateRight45";
        case ActionRequest::RotateLeft90: return "RotateLeft90";
        case ActionRequest::RotateRight90: return "RotateRight90";
        default: return "UnknownRotation";
    }
}

void GameManager::handleShoot(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm) {
    if (!tank->isAlive()) {
        tank->addAction("Shoot (killed)");
        return;
    }

    int wait = tank->getWaitingToShootAgain();
    if (wait != -1 || tank->getNumBullets() == 0) {
        tank->addAction("Shoot (ignored)");
        if (wait != -1) tank->setWaitingToShootAgain(wait - 1);
        return;
    }

    tank->addAction("Shoot");
    tank->shoot();

    auto [dx, dy] = directionToVector(tank->getCanonDirection());
    int shell_x = (tank->getX() + dx + shared_board->getWidth()) % shared_board->getWidth();
    int shell_y = (tank->getY() + dy + shared_board->getHeight()) % shared_board->getHeight();

    shared_board->addShell(*(std::make_shared<Shell>(shell_x, shell_y, tank->getCanonDirection())));
    tank->setWaitingToShootAgain(4);
    tank->setNextPosition(tank->getX(), tank->getY(), tank->getCanonDirection(), 0,
                          shared_board->getWidth(), shared_board->getHeight());
    tank_algorithm.setHaveBattleInfo(false);

    if (numUsefulShells(player1) == 0 && numUsefulShells(player2) == 0){
        out_of_emo = true;
        moves_left = 40;
    }
}

int GameManager::numUsefulShells(Algorithm_211466123_212399455::Player1& p1){
    int sum = 0;
    for(auto tank : p1.getTanks()){
        if(!tank->isAlive()) continue;
        sum += tank->getNumBullets();
    }
    return sum;
}

int GameManager::numUsefulShells(Algorithm_211466123_212399455::Player2& p2){
        int sum = 0;
    for(auto tank : p2.getTanks()){
        if(!tank->isAlive()) continue;
        sum += tank->getNumBullets();
    }
    return sum;
}


void GameManager::handleDoNothing(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm) {
    if (tank->getWaitingToGoBack() != -1) {
        tank->addAction("DoNothing (ignored)");
        tank->setWaitingToGoBack(tank->getWaitingToGoBack() - 1);
    } else if (tank->isAlive()) {
        tank->addAction("DoNothing");
        tank_algorithm.setHaveBattleInfo(false);
    } else {
        tank->addAction("DoNothing (killed)");
    }
}

void GameManager::updateGame() {
    if (game_over) return;
    
    auto& player1_algorithms = this->player1.getTankAlgorithms();
    auto& player1_tanks = this->player1.getTanks();
    
    for(size_t i = 0; i < player1_algorithms.size() && i < player1_tanks.size(); i++) {
        auto& tank_algorithm = player1_algorithms[i];
        auto& tank = player1_tanks[i];
        if (tank && tank_algorithm) {
            tank->setPreviousPosition();
            ActionRequest action1 = tank_algorithm->getAction();
            processAction(tank, *tank_algorithm, action1);
        }
    }
    auto& player2_algorithms = this->player2.getTankAlgorithms();
    auto& player2_tanks = this->player2.getTanks();
    
    for(size_t i = 0; i < player2_algorithms.size() && i < player2_tanks.size(); i++) {
        auto& tank_algorithm = player2_algorithms[i];
        auto& tank = player2_tanks[i];
        if (tank && tank_algorithm) {
            tank->setPreviousPosition();
            ActionRequest action2 = tank_algorithm->getAction();
            processAction(tank, *tank_algorithm, action2);
        }
    }

}

void GameManager::run() {
    std::ofstream output_file(output_file_name);
    if (!output_file) {
        std::cerr << "Failed to open Output file.\n";
        return;
    }

    if (checkInitialTanks(output_file)) return;

    int step = 0;
    while (!isGameOver() && moves_left > 0) {
        int num_tanks_player1 = countAliveTanks(player1);
        int num_tanks_player2 = countAliveTanks(player2);

        resolveShellCollisions();
        updateShells();
        shared_board->displayBoard();

        if (checkEndRound(num_tanks_player1, num_tanks_player2)) {
            endGame();
        }

        if (step % 2 == 0) {
            updateGame();
            resolveShellCollisions();
            resolveTankCollisions();
            updateTanks();

            if (checkEndRound(num_tanks_player1, num_tanks_player2)){
                endGame();
            }
        }

        step++;

        if (--moves_left == 0) {
            if(out_of_emo){
                wining_tank = '3';
                endGame();
            }
            else{
                wining_tank = '0';
                endGame();
            }

        }
    }
    writeTankActions(output_file, player1);
    writeTankActions(output_file, player2);
    writeFinalResult(output_file);
    output_file.close();
    std::cout << "Finished writing to Output.txt successfully.\n";
}

bool GameManager::checkInitialTanks(std::ofstream& out) {
    if (player1.getNumTanks() == 0 && player2.getNumTanks() == 0) {
        std::cout << "Tie, both players have zero tanks\n";
        out << "Tie, both players have zero tanks\n";
        wining_tank = '0'; endGame(); return true;
    } else if (player1.getNumTanks() == 0) {
        std::cout << "Game Over: Player 1 has no tanks left!\n";
        out << "Player 2 won with " << player2.getTanks().size() << " tanks still alive\n";
        wining_tank = '2'; endGame(); return true;
    } else if (player2.getNumTanks() == 0) {
        std::cout << "Game Over: Player 2 has no tanks left!\n";
        out << "Player 1 won with " << player1.getTanks().size() << " tanks still alive\n";
        wining_tank = '1'; endGame(); return true;
    }
    return false;
}

bool GameManager::checkEndRound(int t1, int t2) {
    if (t1 == 0 && t2 == 0) {
        std::cout << "Game Over: Both players have no tanks left!\n";
        wining_tank = '0'; endGame(); return true;
    } else if (t1 == 0) {
        std::cout << "Game Over: Player 1 has no tanks left!\n";
        wining_tank = '2'; endGame(); return true;
    } else if (t2 == 0) {
        std::cout << "Game Over: Player 2 has no tanks left!\n";
        wining_tank = '1'; endGame(); return true;
    }
    return false;
}

int GameManager::countAliveTanks(Algorithm_211466123_212399455::Player1& player) {
    int count = 0;
    for (auto& tank : player.getTanks()) {
        if (tank && tank->isAlive()) count++;
    }
    return count;
}

int GameManager::countAliveTanks(Algorithm_211466123_212399455::Player2& player) {
    int count = 0;
    for (auto& tank : player.getTanks()) {
        if (tank && tank->isAlive()) count++;
    }
    return count;
}

void GameManager::writeTankActions(std::ofstream& out, Player& player) {
    std::vector<std::shared_ptr<Tank>> tanks;
    if(player.getPlayerIndex() == 1){
        Algorithm_211466123_212399455::Player1* p1 = dynamic_cast<Algorithm_211466123_212399455::Player1*>(&player);
        if(p1){
            tanks = p1->getTanks();
        }
    }
    else if(player.getPlayerIndex() == 2){
        Algorithm_211466123_212399455::Player2* p2 = dynamic_cast<Algorithm_211466123_212399455::Player2*>(&player);
        if(p2){
            tanks = p2->getTanks();
        }
    }
    for (auto& tank : tanks) {
        if (!tank) continue;
        auto& actions = tank.get()->getActions();
        if (actions.empty()) {
            out << " has no actions.\n";
            continue;
        }
        for (size_t i = 0; i + 1 < actions.size(); i++)
            out << actions[i] << ",";
        out << actions.back() << "\n";
    }
}

void GameManager::writeFinalResult(std::ofstream& out) {
    int p1 = countAliveTanks(player1);
    int p2 = countAliveTanks(player2);
    if (wining_tank == '1')
        out << "Player 1 won with " << p1 << " still alive\n";
    else if (wining_tank == '2')
        out << "Player 2 won with " << p2 << " still alive\n";
    else if(wining_tank == '3')
        out<<"Tie, both players have zero shells for "<<40<<" steps\n";
    else
        out << "Tie, reached max steps = " << max_steps << ", player 1 has " << p1
            << ", player2 has " << p2 << "\n";
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
    std::ofstream file_errors("input_errors.txt");
    if (!file_errors) return logFileOpenError(filename);

    logBoardProcessingStart(file_errors, filename);
    if (!parseConfig(file_board, file_errors, filename)) return false;

    initMineGridAndMoves();
    if (!validateBoardDimensions(file_errors, filename)) return false;

    auto board = parseBoardLayout(file_board);
    if (board.empty()) return false;

    initializePlayers();
    assignTanksAndAlgorithms(board);

    setupSharedBoard(board);
    displayAndLogBoard(file_errors, filename);
    return true;
}

bool GameManager::logFileOpenError(const std::string& filename) {
    std::cerr << "Failed to open file for writing input errors from " << filename << std::endl;
    return false;
}

void GameManager::logBoardProcessingStart(std::ofstream& errors, const std::string& filename) {
    std::cout << "Opening file for writing input errors from: " << filename << std::endl;
    errors << "Processing board file: " << filename << "\n===========================================" << std::endl;
}

bool GameManager::parseConfig(std::istream& in, std::ofstream& errors, const std::string& filename) {
    std::regex max_steps_re(R"(MaxSteps\s*=\s*(\d+))"),
               shells_re(R"(NumShells\s*=\s*(\d+))"),
               rows_re(R"(Rows\s*=\s*(\d+))"),
               cols_re(R"(Cols\s*=\s*(\d+))");
    std::string line;
    std::getline(in, line);
    if (line.empty()) {
        errors << "Error in " << filename << ": Empty file or missing configuration parameters." << std::endl;
        std::cerr << "Empty file or missing configuration parameters in " << filename << std::endl;
        return false;
    }
    for (int i = 0; i < 4; i++) {
        std::getline(in, line);
        if (line.empty()) {
            continue; // Skip empty lines
        }
        std::smatch match;
        if (std::regex_search(line, match, max_steps_re)) max_steps = std::stoi(match[1]);
        else if (std::regex_search(line, match, shells_re)) num_shells = std::stoi(match[1]);
        else if (std::regex_search(line, match, rows_re)) height = std::stoi(match[1]);
        else if (std::regex_search(line, match, cols_re)) width = std::stoi(match[1]);
        else {
            errors << "Error in " << filename << " (line " << (i + 1) << "): Invalid line: " << line << std::endl;
            return false;
        }
    }
    return true;
}

void GameManager::initMineGridAndMoves() {
    is_mine.assign(height, std::vector<bool>(width, false));
    moves_left = max_steps;
}

bool GameManager::validateBoardDimensions(std::ofstream& errors, const std::string& filename) {
    if (width <= 0 || height <= 0) {
        errors << "Error in " << filename << ": Invalid board dimensions (w: " << width << ", h: " << height << ")" << std::endl;
        return false;
    }
    return true;
}

std::vector<std::vector<std::shared_ptr<Shape>>> GameManager::parseBoardLayout(std::istream& in) {
    std::vector<std::vector<std::shared_ptr<Shape>>> board(height, std::vector<std::shared_ptr<Shape>>(width));
    player1 = Algorithm_211466123_212399455::Player1(1, width, height, max_steps, num_shells, 0);
    player2 = Algorithm_211466123_212399455::Player2(2, width, height, max_steps, num_shells, 0);

    for (size_t i = 0; i < height; i++) {
        std::string line;
        std::getline(in, line);
        for (size_t j = 0; j < width; j++) {
            char c = (j < line.size()) ? line[j] : ' ';
            board[i][j] = createCell(c, j, i);
        }
    }
    return board;
}

std::shared_ptr<Shape> GameManager::createCell(char c, int x, int y) {
    if (c == '1') {
        auto t = std::make_shared<Tank>(x, y, '1');
        player1.addTank(t);
        return t;
    } else if (c == '2') {
        auto t = std::make_shared<Tank>(x, y, '2');
        player2.addTank(t);
        return t;
    } else if (c == '#') return std::make_shared<Wall>(x, y);
    else if (c == '@') {
        is_mine[y][x] = true;
        return std::make_shared<Mine>(x, y);
    }
    return std::make_shared<Empty>(x, y);
}

void GameManager::initializePlayers() {
    player1.setNumTanks(player1.getTanks().size());
    player2.setNumTanks(player2.getTanks().size());
}

void GameManager::assignTanksAndAlgorithms(const std::vector<std::vector<std::shared_ptr<Shape>>>& board) {
    int a1 = 0, a2 = 0;
    for (size_t i = 0; i < height; i++)
        for (size_t j = 0; j < width; j++) {
            auto& cell = board[i][j];
            if (!cell) continue;
            if (cell->getCellType() == CellType::TANK1) {
                auto t = std::dynamic_pointer_cast<Tank>(cell);
                player1.addTankAlgorithm(getTankAlgorithmFactory()->create(1, a1++));
            } else if (cell->getCellType() == CellType::TANK2) {
                auto t = std::dynamic_pointer_cast<Tank>(cell);
                player2.addTankAlgorithm(getTankAlgorithmFactory()->create(2, a2++));
            }
        }
}

void GameManager::setupSharedBoard(const std::vector<std::vector<std::shared_ptr<Shape>>>& board) {
    shared_board = std::make_shared<GameBoard>(width, height, board, is_mine);
}

void GameManager::displayAndLogBoard(std::ofstream& errors, const std::string& filename) {
    shared_board->displayBoard();
    errors << "Successfully loaded board from " << filename << std::endl;
    errors << "Player 1 tanks: " << player1.getTanks().size() << std::endl;
    errors << "Player 2 tanks: " << player2.getTanks().size() << std::endl;
}

bool GameManager::isGameOver() const {
    return game_over;
}

void GameManager::displayGame() const {
    // Implementation for displaying the game state
    std::cout << "=== GAME STATE ===" << std::endl;
    if (shared_board) {
        shared_board->displayBoard();
    }
    int num_tanks_player1 = 0;
    int num_tanks_player2 = 0;
    for (const auto& tank : player1.getTanks()) {
        if (tank && tank->isAlive()) {
            num_tanks_player1++;
        }
    }
    for (const auto& tank : player2.getTanks()) {
        if (tank && tank->isAlive()) {
            num_tanks_player2++;
        }
    }
    std::cout << "Player 1 tanks: " << num_tanks_player1 << std::endl;
    std::cout << "Player 2 tanks: " << num_tanks_player2 << std::endl;
    std::cout << "Moves left: " << moves_left << std::endl;
    std::cout << "=================" << std::endl;
} 

void GameManager::setOutputFileNameFromInput(const std::string& inputFileName) {
    // Extract just the filename (no directories)
    std::string cleanName = std::filesystem::path(inputFileName).filename().string();
    output_file_name = "output_" + cleanName;
}
}
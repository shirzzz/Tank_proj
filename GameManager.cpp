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

// Track tanks after removal for reporting
Tank* lastKnownTank1 = nullptr;
Tank* lastKnownTank2 = nullptr;

void GameManager::readBoard(std::istream& file_board, std::string filename) {
    if (!shared_board->loadBoardFromFile(file_board, filename)) {
        std::cerr << "Error loading board from file." << std::endl;
        return;
    }
    shared_board->displayBoard();
    if (tank1 == nullptr) {
        tank1 = shared_board->getTank1();
    }
    if (tank2 == nullptr) {
        tank2 = shared_board->getTank2();
    }
}


void GameManager::displayGame() const {
    std::cout << "Tank 1 Actions:\n";
    if (lastKnownTank1) {
        for (const auto& action : lastKnownTank1->getActions()) {
            std::cout << action << std::endl;
        }
    }

    std::cout << "Tank 2 Actions:\n";
    if (lastKnownTank2) {
        for (const auto& action : lastKnownTank2->getActions()) {
            std::cout << action << std::endl;
        }
    }

    if (!shared_board->getTank1() && lastKnownTank1) {
        std::cout << "Tank 1 was destroyed. Cause: " << lastKnownTank1->getDestructionCauseStr() << std::endl;
    }

    if (!shared_board->getTank2() && lastKnownTank2) {
        std::cout << "Tank 2 was destroyed. Cause: " << lastKnownTank2->getDestructionCauseStr() << std::endl;
    }
}

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
        auto cell = shared_board->getCell(x, y);

        if (cell->getCellType() == CellType::WALL) {
            Wall* wall = static_cast<Wall*>(cell.get());
            wall->setLives(wall->getLives() - 1);
            if (wall->getLives() <= 0) {
                shared_board->removeWall(wall);
                toExplode.push_back(wall);
            }
            toExplode.push_back(&shell);
        } else if (cell->getCellType() == CellType::TANK1 || cell->getCellType() == CellType::TANK2) {
            std::shared_ptr<Tank> hit_tank = shared_board->getCell(x, y);
            if (hit_tank) {
                auto player = shared_board->getPlayerByTank(hit_tank);
                hit_tank->killTank();
                player->addKilledTank(hit_tank->getTankIndex());
                player->setNumKilledTanks(player->getNumKilledTanks() + 1);
                shared_board->removeTank(hit_tank);
                player->removeTank(hit_tank);
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
    auto player1Tanks = shared_board->getPlayer1()->getTanks();
    auto player2Tanks = shared_board->getPlayer2()->getTanks();

    for (auto& tank1 : player1Tanks) {
        for (auto& tank2 : player2Tanks) {
            if (!tank1 || !tank2) continue;
            auto p1 = tank1->getPosition();
            auto p2 = tank2->getPosition();
            auto o1 = tank1->getPreviousPosition();
            auto o2 = tank2->getPreviousPosition();

            if (p1 == p2 || (p1 == o2 && p2 == o1)) {
                auto player1 = shared_board->getPlayerByTank(tank1);
                auto player2 = shared_board->getPlayerByTank(tank2);
                player1->addKilledTank(tank1->getTankIndex());
                player1->setNumKilledTanks(player1->getNumKilledTanks() + 1);
                shared_board->removeTank(tank1);
                player1->removeTank(tank1);
                tank1->killTank();
                tank2->killTank();
                player2->addKilledTank(tank2->getTankIndex());
                player2->setNumKilledTanks(player2->getNumKilledTanks() + 1);
                shared_board->removeTank(tank2);
                player2->removeTank(tank2);
            }
        }
    }

    auto allTanks = shared_board->getAllTanks();
    for (auto& tank : allTanks) {
        if (!tank) continue;
        auto [x, y] = tank->getPosition();
        if (shared_board->getCell(x, y)->getCellType() == CellType::MINE) {
            auto player = shared_board->getPlayerByTank(tank);
            player->addKilledTank(tank->getTankIndex());
            tank->killTank();
            player->setNumKilledTanks(player->getNumKilledTanks() + 1);
            shared_board->removeTank(tank);
            player->removeTank(tank);
            shared_board->setCell(x, y, std::make_shared<Empty>(x, y));
        }
    }

    auto& shells = shared_board->getShells();
    for (const Shell& shell : shells) {
        auto [sx, sy] = shell.getPosition();
        for (auto& tank : allTanks) {
            if (tank && tank->getPosition() == std::make_pair(sx, sy)) {
                auto player = shared_board->getPlayerByTank(tank);
                player->addKilledTank(tank->getTankIndex());
                player->setNumKilledTanks(player->getNumKilledTanks() + 1);
                shared_board->removeTank(tank);
                player->removeTank(tank);
                tank->killTank();
                shared_board->removeShellAtfromBoard(sx, sy);
                break;
            }
        }
    }

    endGame();
}



void GameManager::processAction(std::shared_ptr<Tank> tank, ActionRequest action, const std::string& name) {
    int waiting_to_go_back = tank->getWaitingToGoBack();
    int waiting_to_shoot = tank->getWaitingToShootAgain();
    
    if (action == ActionRequest::GetBattleInfo) {
        MySatelliteView view(*shared_board);
        BattleInfo info(tank->getX(), tank->getY(), tank->getCanonDirection(), view);
        tank->updateTankWithBattleInfo(info);
        ActionRequest newAction = tank->getAlgorithm()->getAction();
        if (newAction != ActionRequest::GetBattleInfo) {
            processAction(tank, newAction, name);
        }
        return;
    }
    if (waiting_to_go_back == 0) {

        std::pair<int, int> new_position = tank->moveBackward(shared_board->getWidth(), shared_board->getHeight());
        if(shared_board->isCellWalkable(new_position.first, new_position.second)){
            tank->setWaitingToGoBack(-1);
            if(tank->isAlive()){
                tank->addAction("MoveBackward");
                shared_board->moveTank(tank->getIndexTank(), new_position.first, new_position.second);
            }
            else {
                tank->addAction("MoveBackward (killed)");
            }
        }
        else if(shared_board->isSteppingWall(new_position.first, new_position.second)){
            tank->addAction("MoveBackward (Ignored)");
        }
    }

        /* Shir: we dont need this I believe*/
        // else if(shared_board->isSteppingMine(new_position.first, new_position.second)){
        //     shared_board->removeTank(tank);
        //     tank->killTank();
        // }
        

    switch (action) {
        case ActionRequest::MoveForward:
            if (waiting_to_go_back >= 1) tank->setWaitingToGoBack(-1);
            else {
                std::pair<int, int> new_position = tank->moveForward(shared_board->getWidth(), shared_board->getHeight());
                if(shared_board->isCellWalkable(new_position.first, new_position.second)){
                    if(tank->isAlive()) {
                        tank->setWaitingToGoBack(-1);
                        tank->addAction("MoveForward");
                        shared_board->moveTank(tank->getIndexTank(), new_position.first, new_position.second);
                    } else {
                        tank->addAction("MoveForward (killed)");
                    }    
                }
                else if(shared_board->isSteppingWall(new_position.first, new_position.second)){
                    tank->addAction("MoveForward (Ignored)");
                }
                // Shir: we dont need this I believe
                // else if(shared_board->isSteppingMine(new_position.first, new_position.second)){
                //     shared_board->removeTank(tank);
                //     tank->killTank();
            }
            break;

        case ActionRequest::MoveBackward:
            if (waiting_to_go_back >= 1) {
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
                if(tank->isAlive())
                    tank->addAction("Shoot (ignored)");
                    tank->setWaitingToShootAgain(waiting_to_shoot - 1);
                else
                    tank->addAction("Shoot (killed)");
                
            } else if (tank->getNumBullets() == 0) {
                if(tank->isAlive())
                    tank->addAction("Shoot (ignored)");
                else
                    tank->addAction("Shoot (killed)");

                //Shir: I think we need to handle the case where all the tanks are out of ammo
                if(tank->getIndexTank() == '1'){
                    player1->player1Shoot();

                }
                else if(tank->getIndexTank() == '2'){
                    player2->player1Shoot();
                }
                if(player1->getSumShells() == 0 && player2->getSumShells() == 0) {
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
    }
}

void GameManager::updateGame() {
    if (game_over) return;
    Player1 player1 = shared_board->getPlayer1();
    Player2 player2 = shared_board->getPlayer2();
    for(auto& tank : player1.getTanks()) {
        if (tank) {
            tank->setPreviousPosition();
            ActionRequest action1 = shared_board->movingAlgorithm(tank);
            processAction(tank, action1, "Tank 1");
        }
    }
    for(auto& tank : player2.getTanks()) {
        if (tank) {
            tank->setPreviousPosition();
            ActionRequest action2 = shared_board->movingAlgorithm(tank);
            processAction(tank, action2, "Tank 2");
        }
    }
}

void GameManager::removeTank(char index) {
    if (index == '1') {
        shared_board->removeTank(tank1);
        lastKnownTank1 = nullptr;
    } else if (index == '2') {
        shared_board->removeTank(tank2);
        lastKnownTank2 = nullptr;
    }
}

void GameManager::run() {
    int step = 0;
    std::ofstream file("Output_" + filename + ".txt");
    if (!file) {
        std::cerr << "Failed to open Output file for writing." << std::endl;
        return 1;
    }
    if(shared_board->getPlayer1()->getTanks().size() == 0 && shared_board->getPlayer2()->getTanks().size() == 0){
        std::cout << "Tie, both players have zero tanks " << std::endl;
        file << "Tie, both players have zero tanks" << std::endl;
        wining_tank = '0';
        endGame();
        return;
    }

    else if(shared_board->getPlayer1()->getTanks().size() == 0){
        wining_tank = '2';
        std::cout << "Game Over: Player 1 has no tanks left!" << std::endl;
        file << "Player 2 won with " <<shared_board->getPlayer2()->getTanks().size()<<"tanks still alive"<<std::endl;
        endGame();
        return;
    }
    else if(shared_board->getPlayer2()->getTanks().size() == 0){
        wining_tank = '1';
        std::cout << "Game Over: Player 2 has no tanks left!" << std::endl;
        file<< "Player 1 won with " <<shared_board->getPlayer1()->getTanks().size()<<"tanks still alive"<<std::endl;
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
    
    for(int i=0; i<step; i++){
        for(auto& tank : player1.getTanks()) {
            for(int j = 0; j < tank->getActions().size() - 1; j++) {
                file << tank->getActions()[j] << ",";
            }
        file << tank->getActions().back() << " ";
        file << std::endl;
    }
    for(auto& tank : player2.getTanks()) {
       for(int j = 0; j < tank->getActions().size() -1; j++) {
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
    std::cout << "Finished writing to Output_" << filename << ".txt successfully." << std::endl;
    }
}

    bool GameManager::isGameEnded() const {
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



#include "GameManager.h"
#include "GameBoard.h"
#include "Tank.h"
#include "Shell.h"
#include "DestructionCause.h"
#include "Chased.h"
#include "DirectionUtils.h"
#include "common/ActionRequest.h"
#include "CanonDirection.h"
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
            std::shared_ptr<Tank> hitTank = shared_board->getCell(x, y);
            if (hitTank) {
                auto player = shared_board->getPlayerByTank(hitTank);
                player->addKilledTank(hitTank->getTankIndex());
                player->setNumKilledTanks(player->getNumKilledTanks() + 1);
                shared_board->removeTank(hitTank);
                player->removeTank(hitTank);
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
            tank->addAction(ActionRequest::MoveBackward);
            shared_board->moveTank(tank->getIndexTank(), new_position.first, new_position.second);
        }
        else if(shared_board->isSteppingWall(new_position.first, new_position.second)){
            tank->addAction(ActionRequest::INVALID_ACTION);
        }
        else if(shared_board->isSteppingMine(new_position.first, new_position.second)){
            shared_board->removeTank(tank);
            tank->addActionActionRequest::LOSE);
            tank->setDestructionCause(DestructionCause::MINE);
            if(tank->getIndexTank() == '1'){
                tank2->setDestructionCause(DestructionCause::MINEOPPONENT);
            } else if(tank->getIndexTank() == '2'){
                tank1->setDestructionCause(DestructionCause::MINEOPPONENT);
            }
            endGame();
            return;
        }
        }

    switch (action) {
        case ActionRequest::MoveForward:
            if (waiting_to_go_back >= 1) tank->setWaitingToGoBack(-1);
            else {
                std::pair<int, int> new_position = tank->moveForward(shared_board->getWidth(), shared_board->getHeight());
                if(shared_board->isCellWalkable(new_position.first, new_position.second)){
                    tank->addAction(ActionRequest::MoveForward);
                    shared_board->moveTank(tank->getIndexTank(), new_position.first, new_position.second);
                }
                else if(shared_board->isSteppingWall(new_position.first, new_position.second)){
                    tank->addAction(ActionRequest::INVALID_ACTION);
                }
                else if(shared_board->isSteppingMine(new_position.first, new_position.second)){
                    shared_board->removeTank(tank);
                    tank->addAction(ActionRequest::LOSE);
                    tank->setDestructionCause(DestructionCause::MINE);
                    if(tank->getIndexTank() == '1'){
                        tank2->setDestructionCause(DestructionCause::MINEOPPONENT);
                    } else if(tank->getIndexTank() == '2'){
                        tank1->setDestructionCause(DestructionCause::MINEOPPONENT);
                    }
                    endGame();
                } 
            }
            break;

        case ActionRequest::MoveBackward:
            if (waiting_to_go_back >= 1) {
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            } 
            break;

        case ActionRequest::RotateLeft45:
            if (waiting_to_go_back == -1) {
                tank->rotateEighthLeft();
                tank->addAction(action);
            }
            else {
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            }
            break;
        case ActionRequest::RotateRight45:
            if (waiting_to_go_back == -1) {
                tank->rotateEighthRight();
                tank->addAction(action);
            }
            else {
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            }
            break;
        case ActionRequest::RotateLeft90:
            if (waiting_to_go_back == -1) {
                tank->rotateQuarterLeft();
                tank->addAction(action);
            }
            else {
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            }
            break;
        case ActionRequest::RotateRight90:
            if (waiting_to_go_back == -1) {
                tank->rotateQuarterRight();
                tank->addAction(action);
            }
            else {
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            }
            break;

            case ActionRequest::Shoot:
            if (waiting_to_shoot != -1) {
                tank->addAction(ActionRequest::INVALID_ACTION);
                tank->setWaitingToShootAgain(waiting_to_shoot - 1);
            } else if (tank->getNumBullets() == 0) {
                tank->addAction(ActionRequest::INVALID_ACTION);
                if (shared_board->getTank1()->getNumBullets() == 0 &&
                    shared_board->getTank2()->getNumBullets() == 0) {
                    if (moves_left > 40) moves_left = 40;
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
                tank->addAction(ActionRequest::Shoot);
                tank->setWaitingToShootAgain(4);
            }
            break;


        
        case ActionRequest::WIN:
            std::cout << "Game Over: " << name << " wins!" << std::endl;
            tank->addAction(ActionRequest::WIN);
            endGame();
            break;

        case ActionRequest::LOSE:
            std::cout << "Game Over: " << name << " loses!" << std::endl;
            tank->addAction(ActionRequest::LOSE);
            endGame();
            break;

        case ActionRequest::DRAW:
            std::cout << "Game Over: It's a draw!" << std::endl;
            tank->addAction(ActionRequest::DRAW);
            endGame();
            break;

        case ActionRequest::INVALID_ACTION:
            std::cout << "Bad step by " << name << std::endl;
            tank->addAction(ActionRequest::INVALID_ACTION);
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
            ActionRequest action1 = shared_board->movingAlgorithm(*tank1);
            processAction(tank1, action1, "Tank 1");
        }
    }
    for(auto& tank : player2.getTanks()) {
        if (tank) {
            tank->setPreviousPosition();
            ActionRequest action2 = shared_board->movingAlgorithm(*tank2);
            processAction(tank2, action2, "Tank 2");
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
    if(shared_board->getPlayer1()->getTanks().size() == 0 && shared_board->getPlayer2()->getTanks().size() == 0){
        std::cout << "Game Over: Both players have no tanks left!" << std::endl;
        wining_tank = '0';
        endGame();
        
        return;
    }

    //לסדר את הoutpul file למקרה הזה
    else if(shared_board->getPlayer1()->getTanks().size() == 0){
        wining_tank = '2';
        std::cout << "Game Over: Player 1 has no tanks left!" << std::endl;
        endGame();
        return;
    }
    else if(shared_board->getPlayer2()->getTanks().size() == 0){
        wining_tank = '1';
        std::cout << "Game Over: Player 2 has no tanks left!" << std::endl;
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
        moves_left = game_manager.getMovesLeft() - 1;
        if (moves_left == 0) {
            endGame();
            std::cout << "Game Over: No moves left!" << std::endl;
            // *** need to change this part ***
            getTank1()->setDestructionCause(DestructionCause::OUTOFAMMO);
            getTank2()->setDestructionCause(DestructionCause::OUTOFAMMO);
            }
        }
    std::ofstream file("Output_" + filename + ".txt");
    if (!file) {
        std::cerr << "Failed to open Output file for writing." << std::endl;
        return 1;
    }
    for(int i=0; i<step; i++){
        for(auto& tank : player1.getTanks()) {
        if (tank) {
            if(player1.isTankAlive(tank.get()->getIndexTank())){
                file << tank.get()->getActions()[0]<< ",";
                tank.get()->deleteFirstAction();
            }
            else{
                file<<"killed, ";
            }
        }
    }
    for(auto& tank : player2.getTanks()) {
        if (tank) {
            if(player2.isTankAlive(tank.get()->getIndexTank())){
                file << tank.get()->getActions()[0]<< ",";
                tank.get()->deleteFirstAction();
            }
            else{
                file<<"killed, ";
            }
        }
    }
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
    if(tank1.get()->getActions()[tank1.get()->getActions().size()-1] == ActionRequest::LOSE){
        std::cout << "Game Over: Tank 1 loses!" << std::endl;
        tank2.get()->addAction(ActionRequest::WIN);
    }
    else if(tank2.get()->getActions()[tank2.get()->getActions().size()-1] == ActionRequest::LOSE){
        std::cout << "Game Over: Tank 2 loses!" << std::endl;
        tank1.get()->addAction(ActionRequest::WIN);
    }
    else {
        std::cout << "Game Over: It's a draw!" << std::endl;
        tank1.get()->addAction(ActionRequest::DRAW);
        tank2.get()->addAction(ActionRequest::DRAW);
    }
    displayGame();
    setGameOver(true);
}
//need to delete
void GameManager::checkWinCondition() {
    auto p1Tanks = shared_board->getPlayer1()->getTanks();
    auto p2Tanks = shared_board->getPlayer2()->getTanks();

    if (p1Tanks.empty() && p2Tanks.empty()) {
        wining_tank = '0';
        std::cout << "Game Over: Both players eliminated." << std::endl;
        setGameOver(true);
    } else if (p1Tanks.empty()) {
        wining_tank = '2';
        std::cout << "Game Over: Player 2 wins with " << p2Tanks.size() << " tank(s) remaining." << std::endl;
        setGameOver(true);
    } else if (p2Tanks.empty()) {
        wining_tank = '1';
        std::cout << "Game Over: Player 1 wins with " << p1Tanks.size() << " tank(s) remaining." << std::endl;
        setGameOver(true);
    }
}



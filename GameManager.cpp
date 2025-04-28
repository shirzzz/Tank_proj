#include "GameManager.h"
#include "GameBoard.h"
#include "Tank.h"
#include "Shell.h"
#include "DestructionCause.h"
#include "Chased.h"
#include "DirectionUtils.h"
#include "ActionType.h"
#include "CanonDirection.h"
#include "CellType.h"
#include <iostream>
#include <vector>
#include <string>

// Track tanks after removal for reporting
Tank* lastKnownTank1 = nullptr;
Tank* lastKnownTank2 = nullptr;
void GameManager::endGame() {
    if(tank1.get()->getActions()[tank1.get()->getActions().size()-1] == ActionType::LOSE){
        std::cout << "Game Over: Tank 1 loses!" << std::endl;
        tank2.get()->addAction(ActionType::WIN);
    }
    else if(tank2.get()->getActions()[tank2.get()->getActions().size()-1] == ActionType::LOSE){
        std::cout << "Game Over: Tank 2 loses!" << std::endl;
        tank1.get()->addAction(ActionType::WIN);
    }
    else {
        std::cout << "Game Over: It's a draw!" << std::endl;
        tank1.get()->addAction(ActionType::DRAW);
        tank2.get()->addAction(ActionType::DRAW);
    }
    displayGame();
    setGameOver(true);
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
    // Check for shell-to-shell collisions
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

    // Check shell interactions with walls and tanks
    for (Shell& shell : shells) {
        auto pos = shell.getPosition();
        int x = pos.first;
        int y = pos.second;
        CellType cell = shared_board->getCell(x, y)->getCellType();
        if (cell == CellType::WALL) {
           Wall* wall = static_cast<Wall*>(shared_board->getCell(x, y));
            if (wall) {
                wall->setLives(wall->getLives() - 1);
                if (wall->getLives() <= 0) {
                    shared_board->removeWall(wall);

                    toExplode.push_back(wall);
                }
            }
            toExplode.push_back(&shell);
        } else if (cell == CellType::TANK1 || cell == CellType::TANK2) {
            std::shared_ptr<Tank> target = (cell == CellType::TANK1) ? shared_board->getTank1() : shared_board->getTank2();
            if (target) {
                target->setDestructionCause(DestructionCause::SHELL);

                if (target->getIndexTank() == '1') {
                    tank2->setDestructionCause(DestructionCause::SHELLOPPONENT);
                    lastKnownTank1 = target;
                }
                else {
                    tank1->setDestructionCause(DestructionCause::SHELLOPPONENT);
                    lastKnownTank2 = target;
                }

                target->addAction(ActionType::LOSE);
            }
            toExplode.push_back(&shell);
            shared_board->removeTank(target);
            toExplode.push_back(target.get());
        }
    }

    // Remove all shells that should explode
    for (const auto& shape : toExplode) {
        CellType cell = shape->getCellType();
        switch(cell) {
            case CellType::TANK1:
                shared_board->removeTank(std::shared_ptr<Tank>(static_cast<Tank*>(shape)));
                break;
            case CellType::TANK2:
                shared_board->removeTank(std::shared_ptr<Tank>(static_cast<Tank*>(shape)));
                break;
            case CellType::WALL:
                shared_board->removeWall(static_cast<Wall*>(shape));
                break;
            case CellType::SHELL:
                shared_board->removeShell(*static_cast<Shell*>(shape));
                break;
            default:
                std::cout << "Cant be explode!!" << std::endl;
                break;
        }
    }
}

void GameManager::resolveTankCollisions() {
    if (!tank1 || !tank2) return;

    auto p1 = tank1->getPosition();
    auto p2 = tank2->getPosition();
    auto o1 = tank1->getPreviousPosition();
    auto o2 = tank2->getPreviousPosition();

    if (p1 == p2 || (p1 == o2 && p2 == o1)) {
        tank1->setDestructionCause(DestructionCause::TANK);
        tank2->setDestructionCause(DestructionCause::TANK);
        lastKnownTank1 = tank1;
        lastKnownTank2 = tank2;
        shared_board->removeTank(tank1);
        shared_board->removeTank(tank2);
        tank1->addAction(ActionType::DRAW);
        tank2->addAction(ActionType::DRAW);
        endGame();
        return;
    }

    for (std::shared_ptr<Tank> tank : {tank1, tank2}) {
        auto pos = tank->getPosition();
        int x = pos.first;
        int y = pos.second;
        if (shared_board->getCell(x, y)->getCellType() == CellType::MINE) {
            tank->setDestructionCause(DestructionCause::MINE);
            if(tank->getIndexTank() == '1'){
                lastKnownTank1 = tank;
                }
            else lastKnownTank2 = tank;
           tank->addAction(ActionType::LOSE);
            (shared_board->setCell(x, y, new Empty(x, y)));
            shared_board->removeTank(tank);
            tank->addAction(ActionType::LOSE);
            endGame();
            return;
        }
    }

    // Check if a tank moved into a shell's position
    auto& shells = shared_board->getShells();
    for (const Shell& shell : shells) {
        auto shellPos = shell.getPosition();

        for (std::shared_ptr<Tank> tank : {tank1, tank2}) {
            if (tank && tank->getPosition() == shellPos) {
                tank->setDestructionCause(DestructionCause::SHELL);

            if (tank->getIndexTank() == '1') {
                tank2->setDestructionCause(DestructionCause::SHELLOPPONENT);
                lastKnownTank1 = tank;
            } else {
                tank1->setDestructionCause(DestructionCause::SHELLOPPONENT);
                lastKnownTank2 = tank;
            }

            tank->addAction(ActionType::LOSE);
            shared_board->removeTank(tank);
            shared_board->removeShellAtfromBoard(shellPos.first, shellPos.second);

            endGame();
            return;
        }
    }
}
}

void GameManager::processAction(std::shared_ptr<Tank> tank, ActionType action, const std::string& name) {
    int waiting_to_go_back = tank->getWaitingToGoBack();
    int waiting_to_shoot = tank->getWaitingToShootAgain();

    if (waiting_to_go_back == 0) {

        std::pair<int, int> new_position = tank->moveBackward(shared_board->getWidth(), shared_board->getHeight());
        if(shared_board->isCellWalkable(new_position.first, new_position.second)){
            tank->setWaitingToGoBack(-1);
            tank->addAction(ActionType::MOVE_BACKWARD);
            shared_board->moveTank(tank->getIndexTank(), new_position.first, new_position.second);
        }
        else if(shared_board->isSteppingWall(new_position.first, new_position.second)){
            tank->addAction(ActionType::INVALID_ACTION);
        }
        else if(shared_board->isSteppingMine(new_position.first, new_position.second)){
            shared_board->removeTank(tank);
            tank->addAction(ActionType::LOSE);
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
        case ActionType::MOVE_FORWARD:
            if (waiting_to_go_back >= 1) tank->setWaitingToGoBack(-1);
            else {
                std::pair<int, int> new_position = tank->moveForward(shared_board->getWidth(), shared_board->getHeight());
                if(shared_board->isCellWalkable(new_position.first, new_position.second)){
                    tank->addAction(ActionType::MOVE_FORWARD);
                    shared_board->moveTank(tank->getIndexTank(), new_position.first, new_position.second);
                }
                else if(shared_board->isSteppingWall(new_position.first, new_position.second)){
                    tank->addAction(ActionType::INVALID_ACTION);
                }
                else if(shared_board->isSteppingMine(new_position.first, new_position.second)){
                    shared_board->removeTank(tank);
                    tank->addAction(ActionType::LOSE);
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

        case ActionType::MOVE_BACKWARD:
            if (waiting_to_go_back >= 1) {
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            } 
            break;

        case ActionType::ROTATE_EIGHTH_LEFT:
            if (waiting_to_go_back == -1) {
                tank->rotateEighthLeft();
                tank->addAction(action);
            }
            else {
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            }
            break;
        case ActionType::ROTATE_EIGHTH_RIGHT:
            if (waiting_to_go_back == -1) {
                tank->rotateEighthRight();
                tank->addAction(action);
            }
            else {
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            }
            break;
        case ActionType::ROTATE_QUARTER_LEFT:
            if (waiting_to_go_back == -1) {
                tank->rotateQuarterLeft();
                tank->addAction(action);
            }
            else {
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            }
            break;
        case ActionType::ROTATE_QUARTER_RIGHT:
            if (waiting_to_go_back == -1) {
                tank->rotateQuarterRight();
                tank->addAction(action);
            }
            else {
                tank->setWaitingToGoBack(waiting_to_go_back - 1);
            }
            break;

            case ActionType::SHOOT:
            if (waiting_to_shoot != -1) {
                tank->addAction(ActionType::INVALID_ACTION);
                tank->setWaitingToShootAgain(waiting_to_shoot - 1);
            } else if (tank->getNumBullets() == 0) {
                tank->addAction(ActionType::INVALID_ACTION);
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
                shared_board->addShell(*(new Shell(shell_x, shell_y, tank->getCanonDirection())));
                tank->addAction(ActionType::SHOOT);
                tank->setWaitingToShootAgain(4);
            }
            break;

        case ActionType::WIN:
            std::cout << "Game Over: " << name << " wins!" << std::endl;
            tank->addAction(ActionType::WIN);
            endGame();
            break;

        case ActionType::LOSE:
            std::cout << "Game Over: " << name << " loses!" << std::endl;
            tank->addAction(ActionType::LOSE);
            endGame();
            break;

        case ActionType::DRAW:
            std::cout << "Game Over: It's a draw!" << std::endl;
            tank->addAction(ActionType::DRAW);
            endGame();
            break;

        case ActionType::INVALID_ACTION:
            std::cout << "Bad step by " << name << std::endl;
            tank->addAction(ActionType::INVALID_ACTION);
            break;
    }
}

void GameManager::updateGame() {
    if (game_over) return;
    std::shared_ptr<Tank> tank1 = shared_board->getTank1();
    std::shared_ptr<Tank> tank2 = shared_board->getTank2();
    if (!tank1 || !tank2) return;
    tank1->setPreviousPosition();
    tank2->setPreviousPosition();
    ActionType action1 = shared_board->movingAlgorithm(*tank1);
    processAction(tank1, action1, "Tank 1");
    ActionType action2 = shared_board->movingAlgorithm(*tank2);
    processAction(tank2, action2, "Tank 2");
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

#include "GameManager.h"
#include "GameBoard.h"
#include "Tank.h"
#include "Shell.h"
#include <iostream>
#include <vector>
#include <string>
#include "BFSChaserAI.h"
#include "Chased.h"
#include "DirectionUtils.h"
#include "ActionType.h"
#include "ActionType.cpp"
#include "CanonDirection.h"
#include "CellType.h"
#include "GameManager.h"

void GameManager::endGame() {
    displayGame();
    game_over = true;
}

void GameManager::displayGame() {
    std::cout << "Tank 1 Actions:" << std::endl;
    for (const auto& action : this->getTank1()->getActions()) {
        std::cout << action << std::endl;
    }
    std::cout << "Tank 2 Actions:" << std::endl;
    for (const auto& action : this->tank2->getActions()) {
        std::cout << action << std::endl;
    }
    std::cout << "Game Over: " << (game_over ? "Yes" : "No") << std::endl;
}


void GameManager::updateShells() {
    auto& shells = game_board.getShells(); // Reference, not copy!

    for (size_t i = 0; i < shells.size(); ) {
        Shell& shell = shells[i];
        shell.moveShell();
        std::pair<int, int> current_position = shell.getPosition();

        CellType cell = game_board.getCell(current_position.first, current_position.second);

        if (cell == CellType::WALL) {
            Wall* wall = game_board.getWall(current_position.first, current_position.second);
            if (wall) {
                wall->setLives(wall->getLives() - 1);
                if (wall->getLives() <= 0) {
                    game_board.removeWall(wall);
                }
            }
            game_board.removeShell(shell);
            // Don't increment i — shell list might have shifted due to erase
        } else if (cell == CellType::TANK1 || cell == CellType::TANK2) {
            endGame();
            game_board.removeShell(shell);
        } else {
            ++i;
        }
    }
}

void GameManager::processAction(Tank* tank, ActionType action, const std::string& name) {
    int waiting_to_go_back = tank->getWatingToGoBack();
    int waiting_to_shoot_again = tank->getWaitingToShootAgain();
    if (waiting_to_go_back == 0) {
        waiting_to_go_back = -1;
        tank->setWaitingToGoBack(waiting_to_go_back);
        tank->addAction(ActionType::MOVE_BACKWARD);
        tank->moveBackward();
        return;
    }
    switch (action) {
        case ActionType::MOVE_FORWARD:
            if(waiting_to_go_back >= 1) {
                tank->setWaitingToGoBack(-1); // Reset waiting_to_go_back and doing nothing
                break;
            }
             else {
                tank->moveForward();
                 tank->addAction(ActionType::MOVE_FORWARD);
                 break;
            }
        case ActionType::MOVE_BACKWARD:
            if(waiting_to_go_back >= 1) {
                tank->setWaitingToGoBack(waiting_to_go_back--); // Reset waiting_to_go_back and doing nothing
                break;
            }
            else {
                if(tank->getActions()[tank->getActions().size() - 1] == ActionType::MOVE_BACKWARD) { //we can make the tank go back
                    tank->setWaitingToGoBack(-1);
                    tank->addAction(ActionType::MOVE_BACKWARD);
                    tank->moveBackward();
                }
                else {
                    waiting_to_go_back = 4; // Set waiting_to_go_back to 4
                    tank->setWaitingToGoBack(waiting_to_go_back);
                }
                break;
            }

        case ActionType::ROTATE_EIGHTH_LEFT:
            if(waiting_to_go_back == -1) {
                tank->rotateEighthLeft();
                tank->addAction(ActionType::ROTATE_EIGHTH_LEFT);
            }
            else {
                waiting_to_go_back--;
                tank->setWaitingToGoBack(waiting_to_go_back);
            }
            break;
        case ActionType::ROTATE_EIGHTH_RIGHT:
            if(waiting_to_go_back == -1) {
                tank->rotateEighthRight();
                tank->addAction(ActionType::ROTATE_EIGHTH_RIGHT);
            }
            else {
                waiting_to_go_back--;
                tank->setWaitingToGoBack(waiting_to_go_back);
            }
            break;
        case ActionType::ROTATE_QUARTER_LEFT:
            if(waiting_to_go_back == -1) {
                tank->rotateQuarterLeft();
                tank->addAction(ActionType::ROTATE_QUARTER_LEFT);
            }
            else {
                waiting_to_go_back--;
                tank->setWaitingToGoBack(waiting_to_go_back);
            }
            break;
        case ActionType::ROTATE_QUARTER_RIGHT:
            if(waiting_to_go_back == -1) {
                tank->rotateQuarterRight();
                tank->addAction(ActionType::ROTATE_QUARTER_RIGHT);
            }
            else {
                waiting_to_go_back--;
                tank->setWaitingToGoBack(waiting_to_go_back);
            }
            break;
        case ActionType::SHOOT: {
            if(waiting_to_shoot_again != -1) {
                tank->addAction(ActionType::INVALID_ACTION);
                waiting_to_shoot_again--;
                tank->setWaitingToShootAgain(waiting_to_shoot_again);
            }
            else if(tank->getNumBullets() == 0) {
                tank->addAction(ActionType::INVALID_ACTION);
                if(tank1->getNumBullets() ==0 && tank2->getNumBullets() == 0) {
                    tank1->addAction(ActionType::DRAW);
                    tank2->addAction(ActionType::DRAW);
                    game_over = true;
                }
            }
            else {
                tank->shoot();
                tank->addAction(ActionType::SHOOT);
                waiting_to_shoot_again = 4; // Set waiting_to_shoot_again to 4
                tank->setWaitingToShootAgain(waiting_to_shoot_again);
            }
            break;
        }
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
            std::cout << "bad step" << std::endl;
            tank->addAction(ActionType::INVALID_ACTION);
            break;
    }
}


void GameManager::updateGame() {
    if (game_over) return; // Don't update if game ended
    Tank* tank1 = game_board.getTank1();
    Tank* tank2 = game_board.getTank2();
    ActionType action1 = tank1->movingAlgorithm();
    processAction(game_board.getTank1(), action1, "Tank 1");
    ActionType action2 = tank2->movingAlgorithm();
    processAction(game_board.getTank2(), action2, "Tank 2");
}

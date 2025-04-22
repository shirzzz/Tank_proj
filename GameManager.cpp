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
    switch (action) {
        case ActionType::MOVE_FORWARD:
            tank->moveForward();
            tank->addAction(ActionType::MOVE_FORWARD);
            break;
        case ActionType::MOVE_BACKWARD:
            tank->moveBackward();
            tank->addAction(ActionType::MOVE_BACKWARD);
            break;
        case ActionType::ROTATE_EIGHTH_LEFT:
            tank->rotateEighthLeft();
            tank->addAction(ActionType::ROTATE_EIGHTH_LEFT);
            break;
        case ActionType::ROTATE_EIGHTH_RIGHT:
            tank->rotateEighthRight();
            tank->addAction(ActionType::ROTATE_EIGHTH_RIGHT);
            break;
        case ActionType::ROTATE_QUARTER_LEFT:
            tank->rotateQuarterLeft();
            tank->addAction(ActionType::ROTATE_QUARTER_LEFT);
            break;
        case ActionType::ROTATE_QUARTER_RIGHT:
            tank->rotateQuarterRight();
            tank->addAction(ActionType::ROTATE_QUARTER_RIGHT);
            break;
        case ActionType::SHOOT: {
            tank->shoot();
            Shell shell(tank->getX(), tank->getY(), tank->getCanonDirection());
            game_board.addShell(shell);
            tank->addAction(ActionType::SHOOT);
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

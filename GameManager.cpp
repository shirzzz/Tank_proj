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
#include "CanonDirection.h"
#include "CellType.h"

void GameManager::endGame() {
    actions.push_back("Game Over");
    displayGame();
    game_over = true;
}

void displayGame() {
    std::cout << "Tank 1 Actions:" << std::endl;
    for (const auto& action : actions_tank1) {
        std::cout << action << std::endl;
    }
    std::cout << "Tank 2 Actions:" << std::endl;
    for (const auto& action : actions_tank2) {
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
    std::vector<std::string>& tankActions =
        (tank->getIndexTank() == '1') ? actions_tank1 : actions_tank2;

    switch (action) {
        case ActionType::MOVE_FORWARD:
            tank->move_forward();
            tankActions.push_back(name + " moved forward");
            break;
        case ActionType::MOVE_BACKWARD:
            tank->move_backward();
            tankActions.push_back(name + " moved backward");
            break;
        case ActionType::ROTATE_EIGHTH_LEFT:
            tank->rotate_eighth_left();
            tankActions.push_back(name + " rotated eighth left");
            break;
        case ActionType::ROTATE_EIGHTH_RIGHT:
            tank->rotate_eighth_right();
            tankActions.push_back(name + " rotated eighth right");
            break;
        case ActionType::ROTATE_QUARTER_LEFT:
            tank->rotate_quarter_left();
            tankActions.push_back(name + " rotated quarter left");
            break;
        case ActionType::ROTATE_QUARTER_RIGHT:
            tank->rotate_quarter_right();
            tankActions.push_back(name + " rotated quarter right");
            break;
        case ActionType::SHOOT: {
            tank->shoot();
            Shell shell(tank->getX(), tank->getY(), tank->getCanonDirection());
            game_board.addShell(shell);
            tankActions.push_back(name + " shot");
            break;
        }
        case ActionType::INVALID_ACTION:
            std::cout << "bad step" << std::endl;
            tankActions.push_back(name + " made an invalid action");
            break;
    }
}


void GameManager::updateGame() {
    if (game_over) return; // Don't update if game ended

    // Tank 1 (AI Chaser)
    BFSChaserAI ai1;
    ActionType action1 = ai1.decideNextAction(game_board, *game_board.getTank1(), *game_board.getTank2());
    processAction(game_board.getTank1(), action1, "Tank 1");

    // Tank 2 (Chased)
    Chased chased;
    ActionType action2 = chased.decideNextAction(game_board, *game_board.getTank2(), *game_board.getTank1());
    processAction(game_board.getTank2(), action2, "Tank 2");
}

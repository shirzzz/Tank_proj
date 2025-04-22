#include "GameManager.h"
#include "GameBoard.h"
#include "Tank.h"
#include "Shell.h"
#include "DestructionCause.h"
#include "BFSChaserAI.h"
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
    displayGame();
    game_over = true;
}

void GameManager::displayGame() {
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

    std::cout << "Game Over: " << (game_over ? "Yes" : "No") << std::endl;

    if (!game_board.getTank1() && lastKnownTank1) {
        std::cout << "Tank 1 was destroyed. Cause: " << lastKnownTank1->getDestructionCauseStr() << std::endl;
    }

    if (!game_board.getTank2() && lastKnownTank2) {
        std::cout << "Tank 2 was destroyed. Cause: " << lastKnownTank2->getDestructionCauseStr() << std::endl;
    }
}

void GameManager::updateShells() {
    auto& shells = game_board.getShells();
    for (Shell& shell : shells) {
        shell.storePreviousPosition();
    }
    for (Shell& shell : shells) {
        shell.moveShell();
    }
}

void GameManager::resolveShellCollisions() {
    auto& shells = game_board.getShells();
    std::vector<std::pair<int, int>> toExplode;

    // Check for shell-to-shell collisions
    for (size_t i = 0; i < shells.size(); ++i) {
        for (size_t j = i + 1; j < shells.size(); ++j) {
            auto pi = shells[i].getPosition();
            auto pj = shells[j].getPosition();
            auto oi = shells[i].getPreviousPosition();
            auto oj = shells[j].getPreviousPosition();

            if (pi == pj || (pi == oj && pj == oi)) {
                toExplode.push_back(pi);
            }
        }
    }

    // Check shell interactions with walls and tanks
    for (Shell& shell : shells) {
        auto [x, y] = shell.getPosition();
        CellType cell = game_board.getCell(x, y);

        if (cell == CellType::WALL) {
            Wall* wall = game_board.getWall(x, y);
            if (wall) {
                wall->setLives(wall->getLives() - 1);
                if (wall->getLives() <= 0) {
                    game_board.removeWall(wall);
                }
            }
            toExplode.push_back({x, y});
        } else if (cell == CellType::TANK1 || cell == CellType::TANK2) {
            Tank* target = (cell == CellType::TANK1) ? game_board.getTank1() : game_board.getTank2();
            if (target) {
                target->setDestructionCause(DestructionCause::SHELL);
                if (target->getIndexTank() == '1') lastKnownTank1 = target;
                else lastKnownTank2 = target;
            }
            toExplode.push_back({x, y});
        }
    }

    // Remove all shells that should explode
    for (auto [x, y] : toExplode) {
        game_board.removeShellAt(x, y);
    }
}

void GameManager::resolveTankCollisions() {
    Tank* t1 = game_board.getTank1();
    Tank* t2 = game_board.getTank2();

    if (!t1 || !t2) return;

    auto p1 = t1->getPosition();
    auto p2 = t2->getPosition();
    auto o1 = t1->getPreviousPosition();
    auto o2 = t2->getPreviousPosition();

    if (p1 == p2 || (p1 == o2 && p2 == o1)) {
        t1->setDestructionCause(DestructionCause::TANK);
        t2->setDestructionCause(DestructionCause::TANK);
        lastKnownTank1 = t1;
        lastKnownTank2 = t2;
        game_board.removeTank('1');
        game_board.removeTank('2');
        endGame();
        return;
    }

    for (Tank* tank : {t1, t2}) {
        auto [x, y] = tank->getPosition();
        if (game_board.getCell(x, y) == CellType::MINE) {
            tank->setDestructionCause(DestructionCause::MINE);
            if (tank->getIndexTank() == '1') lastKnownTank1 = tank;
            else lastKnownTank2 = tank;

            game_board.getBoard()[y][x] = CellType::EMPTY;
            game_board.removeTank(tank->getIndexTank());
            endGame();
            return;
        }
    }
}

void GameManager::processAction(Tank* tank, ActionType action, const std::string& name) {
    int wtgBack = tank->getWatingToGoBack();
    int wtgShoot = tank->getWaitingToShootAgain();

    if (wtgBack == 0) {
        tank->setWaitingToGoBack(-1);
        tank->addAction(ActionType::MOVE_BACKWARD);
        tank->moveBackward(game_board.getWidth(), game_board.getHeight());
        return;
    }

    switch (action) {
        case ActionType::MOVE_FORWARD:
            if (wtgBack >= 1) tank->setWaitingToGoBack(-1);
            else {
                tank->moveForward(game_board.getWidth(), game_board.getHeight());
                tank->addAction(ActionType::MOVE_FORWARD);
            }
            break;

        case ActionType::MOVE_BACKWARD:
            if (wtgBack >= 1) {
                tank->setWaitingToGoBack(wtgBack - 1);
            } else {
                if (!tank->getActions().empty() && 
                    tank->getActions().back() == ActionType::MOVE_BACKWARD) {
                    tank->setWaitingToGoBack(-1);
                    tank->addAction(ActionType::MOVE_BACKWARD);
                    tank->moveBackward(game_board.getWidth(), game_board.getHeight());
                } else {
                    tank->setWaitingToGoBack(4);
                }
            }
            break;

        case ActionType::ROTATE_EIGHTH_LEFT:
        case ActionType::ROTATE_EIGHTH_RIGHT:
        case ActionType::ROTATE_QUARTER_LEFT:
        case ActionType::ROTATE_QUARTER_RIGHT:
            if (wtgBack == -1) {
                if (action == ActionType::ROTATE_EIGHTH_LEFT) tank->rotateEighthLeft();
                if (action == ActionType::ROTATE_EIGHTH_RIGHT) tank->rotateEighthRight();
                if (action == ActionType::ROTATE_QUARTER_LEFT) tank->rotateQuarterLeft();
                if (action == ActionType::ROTATE_QUARTER_RIGHT) tank->rotateQuarterRight();
                tank->addAction(action);
            } else {
                tank->setWaitingToGoBack(wtgBack - 1);
            }
            break;

        case ActionType::SHOOT:
            if (wtgShoot != -1) {
                tank->addAction(ActionType::INVALID_ACTION);
                tank->setWaitingToShootAgain(wtgShoot - 1);
            } else if (tank->getNumBullets() == 0) {
                tank->addAction(ActionType::INVALID_ACTION);
                if (game_board.getTank1()->getNumBullets() == 0 &&
                    game_board.getTank2()->getNumBullets() == 0) {
                    if (moves_left > 40) moves_left = 40;
                }
            } else {
                tank->shoot();
                game_board.addShell(Shell(tank->getX(), tank->getY(), tank->getCanonDirection()));
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

    Tank* tank1 = game_board.getTank1();
    Tank* tank2 = game_board.getTank2();
    if (!tank1 || !tank2) return;

    tank1->storePreviousPosition();
    tank2->storePreviousPosition();

    ActionType action1 = tank1->movingAlgorithm(game_board);
    processAction(tank1, action1, "Tank 1");

    ActionType action2 = tank2->movingAlgorithm(game_board);
    processAction(tank2, action2, "Tank 2");
}

void GameManager::removeTank(char index) {
    if (index == '1') {
        game_board.removeTank('1');
        lastKnownTank1 = nullptr;
    } else if (index == '2') {
        game_board.removeTank('2');
        lastKnownTank2 = nullptr;
    }
}

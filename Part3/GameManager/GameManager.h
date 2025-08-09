#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#pragma once

#include "../common/ActionRequest.h"
#include "../UserCommon/Tank.h"
#include "../UserCommon/MyPlayerFactory.h"  // CHANGED: Use your concrete factory
#include "../UserCommon/MyTankAlgorithmFactory.h"  // CHANGED: Use your concrete factory
#include "../Algorithm/Player1.h"
#include "../Algorithm/Player2.h"
#include <string>
#include <climits>  // For INT_MAX
#include <memory>
#include "../Algorithm/GameBoard.h"
#include "../UserCommon/Shape.h"
#include <fstream>

namespace GameManager_211466123_212399455{

class GameManager {
private:
    bool out_of_emo = false;
    std::shared_ptr<GameBoard> shared_board; // Shared pointer to the game board
    bool game_over = false;
    std::vector<std::vector<bool>> is_mine;
    // Assignment 2 - Factory references
    MyPlayerFactory* player_factory = nullptr;  // CHANGED: Use concrete type
    MyTankAlgorithmFactory* tank_factory = nullptr;  // CHANGED: Use concrete type
    size_t max_steps = 0; // Maximum steps allowed in the game
    size_t num_shells = 0; // Number of shells available for each tank
    size_t width = 0; // Width of the game board
    size_t height = 0; // Height of the game board
    std::string output_file_name;
    Algorithm_211466123_212399455::Player1 player1;
    Algorithm_211466123_212399455::Player2 player2;
    char wining_tank = '0'; // 0 for draw, 1 for tank1, 2 for tank2
    int moves_left = INT_MAX;

    //Class methods which are only used in this class
    void updateShells() const;
    void updateTanks() const;
    void resolveShellCollisions();
    void detectShellVsShellCollisions(const std::vector<Shell>& shells, std::vector<Shape*>& toExplode);
    void detectShellVsWallOrTank(std::vector<Shell>& shells, std::vector<Shape*>& toExplode);
    void handleShellTankCollisions(std::pair<int, int> next_position, Shell& shell, std::vector<Shape*>& toExplode);
    void processExplosions(std::vector<Shape*>& toExplode);
    void resolveTankCollisions();
    std::vector<std::shared_ptr<Tank>> collectAllTanks();
    void handleTankToTankCollisions(std::vector<std::shared_ptr<Tank>>& all_tanks);
    void handleTankMineCollisions(std::vector<std::shared_ptr<Tank>>& all_tanks);
    void handleTankShellCollisions(std::vector<std::shared_ptr<Tank>>& all_tanks);
    void processAction(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm, ActionRequest action);
    void logActionStart(const std::shared_ptr<Tank>& tank, const TankAlgorithm& tank_algorithm, ActionRequest action);
    void handlePassiveBackward(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm);
    void handleGetBattleInfo(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm);
    void handleMoveForward(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm);
    void handleMoveBackward(std::shared_ptr<Tank> tank);
    void handleRotate(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm, ActionRequest action);
    std::string actionToString(ActionRequest action);
    void handleShoot(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm);
    void handleDoNothing(std::shared_ptr<Tank> tank, TankAlgorithm& tank_algorithm);
    void updateGame();
    bool checkInitialTanks(std::ofstream& out);
    bool checkEndRound(int num_tanks_player1, int num_tanks_player2);
    int countAliveTanks(Algorithm_211466123_212399455::Player1& player);
    int countAliveTanks(Algorithm_211466123_212399455::Player2& player);
    void writeTankActions(std::ofstream& out, Player& player);
    void writeFinalResult(std::ofstream& out);
    void endGame();
    bool loadBoardFromFile(std::istream& file_board, const std::string filename);
    bool logFileOpenError(const std::string& filename);
    //void logBoardProcessingStart(std::ofstream& errors, const std::string& filename);
    bool parseConfig(std::istream& in, vector<std::String>& errors, const std::string& filename);
    void initMineGridAndMoves();
    bool validateBoardDimensions(const std::string& filename);
    std::vector<std::vector<std::shared_ptr<Shape>>> parseBoardLayout(std::istream& in);
    std::shared_ptr<Shape> createCell(char c, int x, int  y);
    void initializePlayers();
    void assignTanksAndAlgorithms(const std::vector<std::vector<std::shared_ptr<Shape>>>& board);
    void setupSharedBoard(const std::vector<std::vector<std::shared_ptr<Shape>>>& board);
    void displayAndLogBoard(std::ofstream& errors, const std::string& filename);
    void displayGame() const;
    bool isGameOver() const;
    int numUsefulShells(Algorithm_211466123_212399455::Player2& p2);
    int numUsefulShells(Algorithm_211466123_212399455::Player1& p1);
    void setGameOver(bool val){game_over = val;}
    int waiting_to_shoot = 4; // Cooldown for shooting

    public:
    // Assignment 2 Constructor - Takes factories as required by assignment
    GameManager(MyPlayerFactory& pf, MyTankAlgorithmFactory& tf)  // CHANGED: Use concrete types
        : player_factory(&pf), tank_factory(&tf) {

    }

    // Assignment 2 - Factory accessors
    MyPlayerFactory* getPlayerFactory() const { return player_factory; }  // CHANGED: Return concrete type
    MyTankAlgorithmFactory* getTankAlgorithmFactory() const { return tank_factory; }  // CHANGED: Return concrete type
    
    // Assignment 2 - Direct filename reading
    void readBoard(const std::string& filename);
    void run();
    void setOutputFileNameFromInput(const std::string& inputFileName);

};

#endif // GAMEMANAGER_H
} // namespace GameManager_211466123_212399455
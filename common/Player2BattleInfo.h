class Player2BattleInfo : public BattleInfo {
public:
    std::vector<Tank> opponents;
    std::shared_ptr<GameBoard> board;
};
    
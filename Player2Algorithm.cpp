std::vector<ActionRequest> Player2Algorithm::decideNextAction(GameBoard& board, const Player1& player1, const Player2& player2) {
    std::vector<ActionRequest> actions;
    std::vector<Tank> tanks = player2.getTanks();
    std::vector<Tank> opponent_tanks = player1.getTanks();
    for (const Tank& tank : tanks) {
        Chased chased;
        ActionRequest action = chased.decideNextAction(board, tank, opponent_tanks[0]);
        actions.push_back(action);
    }
    return actions;
}
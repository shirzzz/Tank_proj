ActionRequest Player2Algorithm::getAction() { 
    // this is not good
    std::vector<Tank> tanks = player2.getTanks();
    Chased chased;
    ActionRequest action = chased.decideNextAction(board, tank, opponent_tanks[0]);
    return action;
}

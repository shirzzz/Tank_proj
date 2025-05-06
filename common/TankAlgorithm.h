// TankAlgorithm.h
#ifndef TANKALGORITHM_H
#define TANKALGORITHM_H

class TankAlgorithm {
public:
    virtual ~TankAlgorithm() {} // Virtual destructor for proper cleanup of derived classes
    virtual ActionReqeuest getAction() = 0;
    virtual void updateBattleInfo(BattleInfo& battleInfo) = 0;
};

#endif //TANKALGORITHM_H



#include "MyBattleInfo.h"

MyBattleInfo::MyBattleInfo() = default;

MyBattleInfo::MyBattleInfo(size_t x, size_t y, int dir, size_t shells, int tankIdx)
    : x(x), y(y), direction(dir), shellsRemaining(shells), tankIndex(tankIdx) {}

void MyBattleInfo::extractSatelliteMatrix(const SatelliteView& view, size_t rows, size_t cols) {
    satelliteMatrix.clear();
    for (size_t row = 0; row < rows; ++row) {
        std::string line;
        for (size_t col = 0; col < cols; ++col) {
            line += view.getObjectAt(col, row);
        }
        satelliteMatrix.push_back(line);
    }
}

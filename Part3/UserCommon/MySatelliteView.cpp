#include "MySatelliteView.h"
#include "CellType.h"

MySatelliteView::MySatelliteView(const Algorithm_211466123_212399455::GameBoard& board) : board(board) {}

char MySatelliteView::getObjectAt(size_t x, size_t y) const {
    //Cast size_t to int for comparison with GameBoard methods
    if (static_cast<int>(y) >= board.getHeight() || static_cast<int>(x) >= board.getWidth()) {
        return '&';
    }

    // Cast size_t to int for getCell parameters and handle const issue
    auto cell = const_cast<Algorithm_211466123_212399455::GameBoard&>(board).getCell(static_cast<int>(x), static_cast<int>(y));
    if (!cell) return ' ';

    switch (cell->getCellType()) {
        case CellType::WALL:   return '#';
        case CellType::TANK1:  return '1';
        case CellType::TANK2:  return '2';
        case CellType::MINE:   return '@';
        case CellType::SHELL:  return '*';
        case CellType::EMPTY:  return ' ';
        default:               return ' ';
    }
}
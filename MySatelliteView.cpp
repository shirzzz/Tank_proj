#include "MySatelliteView.h"
#include "CellType.h"

MySatelliteView::MySatelliteView(const GameBoard& board) : board(board) {}

char MySatelliteView::getObjectAt(size_t x, size_t y) const {
    if (y >= board.getHeight() || x >= board.getWidth()) {
        return '&';
    }

    auto cell = board.getCell(x, y);
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

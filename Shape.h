//
// Created by Owner on 04/04/2025.
//

#ifndef SHAPE_H
#define SHAPE_H

#include <utility>
#include "CellType.h"
#include <memory>
class Shape : public std::enable_shared_from_this<Shape> {
protected:
    int x, y;
    int prev_x, prev_y;
    CellType cell_type;
public:

    virtual ~Shape() = default; // Virtual destructor for proper cleanup
    // virtual Shape* clone() const = 0; // ADD THIS
    Shape(int x, int y, CellType cell_type = CellType::EMPTY) : x(x), y(y), prev_x(x), prev_y(y), cell_type(cell_type) {};
    void move();
    void updatePosition(int new_x, int new_y);
    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int new_x) { x = new_x; }
    void setY(int new_y) { y = new_y; }
    std::pair<int,int> getPosition() const { return {x, y}; }
    std::pair<int,int> getPreviousPosition() const { return {x, y}; }
    CellType getCellType() const { return cell_type; }
    void setCellType(CellType new_cell_type) { cell_type = new_cell_type; }

};



#endif //SHAPE_H

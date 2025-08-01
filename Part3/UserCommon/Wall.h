//
// Created by Owner on 18/04/2025.
//

#ifndef WALL_H
#define WALL_H
#include "Shape.h"
#include <utility>

class Wall :public Shape{
private:
  int lives = 2;

public:
    Wall(int x, int y);
    std::pair<int, int> getPosition(){return {x, y};}
    int getLives() const { return lives; }
    void setLives(int newLives) { lives = newLives; }
};



#endif //WALL_H

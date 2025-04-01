//
// Created by Owner on 01/04/2025.
//

#ifndef SHELL_H
#define SHELL_H



class shell {
    int direction;
    pair<int, int> starting_position;
    enum canon_direction {
        U, UR, R, DR, D, DL, L, UL
    };
    canon_direction canon_dir;
    int speed;
public:
    shell(int x, int y, int direction) : starting_position(x, y), direction(direction), speed(1) {
        // Initialize the shell's position and direction
    }

    void move() {
        // Move the shell in the current direction
        if (direction == 0) starting_position.first += speed; // Move right
        else if (direction == 1) starting_position.first -= speed; // Move left
        else if (direction == 2) starting_position.second += speed; // Move up
        else if (direction == 3) starting_position.second -= speed; // Move down
    }
};






#endif //SHELL_H

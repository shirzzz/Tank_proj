
#ifndef TANK_H
#define TANK_H



class tank {
    int direction;
    pair<int, int> position;
    char index_tank;
    enum canon_direction {
        U, UR, R, DR, D, DL, L, UL
    };
    canon_direction canon_dir;
    int num_bullets;
    int last_time_shoot;
    int last_time_backward;
public:
    tank(int x, int y, char index_tank) : position(x, y), index_tank(index_tank), num_bullets(5), last_time_shoot(0), last_time_backward(0) {
        if(index_tank == '1') {
            direction = 0; // Tank 1 starts facing right
        }
        else {
            direction = 1; // Tank 2 starts facing left
        }
    }

    void move_forward() {
        // Move the tank forward in the current direction
        if (direction == 0) position.first++; // Move right
        else if (direction == 1) position.first--; // Move left
        else if (direction == 2) position.second++; // Move up
        else if (direction == 3) position.second--; // Move down
    }

    void move_backward() {
        // Move the tank backward in the current direction
        if (direction == 0) position.first--; // Move left
        else if (direction == 1) position.first++; // Move right
        else if (direction == 2) position.second--; // Move down
        else if (direction == 3) position.second++; // Move up
    }

    void shoot() {
        if (num_bullets > 0) {
            num_bullets--;
            // Logic to shoot a bullet from the tank's current position and direction
        }
    }

    void rotate_left() {
        direction = (direction + 3) % 4; // Rotate left
    }

    void rotate_right() {
        direction = (direction + 1) % 4; // Rotate right
    }

};



#endif //TANK_H

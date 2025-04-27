Tanks Game Simulator
"Advanced Topics in Programming" — TAU Semester B 2025 — Assignment 1

>>Name and Ids:
*Itai Lifshitz, 211466123
*Shir Zadok, 212399455

>>Overview
An implementation of a deterministic program that simulates a tank battle game on a 2D board for "Advanced Topics in Programming" at TAU. 
Each player controls one tank, aiming to destroy the opponent while avoiding walls, mines, and incoming artillery shells. Player 1 tries to chase player 2 using the BFS algorithm, and tank2 does everything it can to survive. 
The simulation reads a board configuration from an input file, runs the game based on pre-programmed tank algorithms, and outputs the results.

>>Input File Format
The input file is divided into two sections:
The first one includes the Board dimensions (width and height), and the second one includes the "Shapes", of the game which are in other words: the game objects.

## Text file format:
1. The first line of the input file defines the width and height of the board which are the board dimensions. 

The dimensions should be in the following order, seperated by a space:

```
width height
```

Where as:
-`width` = The number of columns (horizontal size) of the game board.

-`height` = The number of rows (vertical size) of the game board.

2. Game Objects ("Shapes") (Subsequent Lines):

Each subsequent line describes the differnts game objects (tanks, walls, mines and empty objects) and their position on the game board.
Within the format: 
```
c1 c2 c3 c4 c5 ... cwidth
```

Where as:
-`ci` = A character which represent a type of game object on the board.
-`1` = Player 1's tank
-`2` = Player 2's tank
-`#` = A wall
-`@` = A mine

# Important Notes:
1. If there are more objects in one line than expected they are being ignored and not joined to the built game board.
2. The game will not start if one of the players has a number of tanks which differ than 1.
3. If there are not enough lines or one or more of the lines are too short the game will not start.

### Example Input File:
 10 8
##########
#         #
# 2        #
#         #
#    1    #
#      @ #
#   #    #
########## 

Tank cannons start by default:

Player 1: Left (L)

Player 2: Right (R)


How to Run
bash
Copy
Edit
tanks_game <game_board_input_file>
<game_board_input_file> — Path to a text file describing the initial board layout.


Game Mechanics
Movement: Forward, backward (delayed), and rotate (1/8 or 1/4 turn).

Shooting:

Each tank starts with 16 shells.

Shells move twice as fast as tanks.

After shooting, a 4-step cooldown is enforced.

Boundaries: Toroidal (wrap-around edges).

Collisions:

Shell hits wall → wall weakens and collapses after two hits.

Shell hits tank → tank destroyed.

Tank on mine → both tank and mine destroyed.

Tank collides with tank → both destroyed.

Algorithms
Each player is controlled by a deterministic algorithm:

Algorithms have full visibility of the board.

Algorithms cannot modify the game state directly — they suggest actions, which are validated by the GameManager.

At least one algorithm must attempt to chase the opponent using a search strategy (DFS or BFS).

Output
Output file:

Records every action and any "bad steps" (invalid moves).

Final game result (win/tie) and reason.

Input Errors:

If recoverable issues are found in the input file, they're reported in input_errors.txt.

Project Structure
GameManager: Controls the game flow, validates actions.

TankAlgorithm: Abstracts the decision-making for each tank.

Board: Represents the game board and entities on it.

Shells: Handle artillery shots and collisions.

Additional Requirements
Error Handling:

Never crash unexpectedly.

Attempt to recover from input issues.

No exit() calls; the program must exit naturally.

Documentation:

Provide a High-Level Design Document (HLD) including:

UML Class Diagram

UML Sequence Diagram

Design considerations

Testing strategy

Bonus Opportunities
Optional additions that may earn bonus points:

Logging system.

Configuration file support.

Automated testing (e.g., Google Test framework).

Visual simulation (optional and external to the main gameplay).

Important: To qualify for a bonus, describe the features clearly in a bonus.txt file.

License
This project is for academic use as part of Tel Aviv University's Advanced Topics in Programming course.



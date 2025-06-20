# Tanks Game Simulator
**Advanced Topics in Programming — TAU Semester B 2025 — Assignment 2**

## 👩🏻‍💻👨🏼‍💻 Authors
- **Itai Lifshitz** —  
- **Shir Zadok** — 

---

## 🗺️ Game Overview
This project implements a deterministic simulation of a tank battle game on a 2D board for the "Advanced Topics in Programming" course at TAU.  
Each player controls a single tank, aiming to destroy the opponent while navigating walls, mines, and incoming artillery shells.

- **Player 1**: Actively chases Player 2 using a **BFS (Breadth-First Search)** algorithm.
- **Player 2**: Focuses on survival, evasion.

The game reads the board configuration from an input file, runs the battle based on predefined algorithms, and outputs the results into text files.

Each tank makes decisions via its own TankAlgorithm. The GameManager orchestrates the game and handles input/output as per assignment specs.
---

## 🔣 Input File Format

**Input Rules:**
All values must appear in the first 5 lines.
The map starts from line 6.
Symbols:
# = Wall
@ = Mine
1 = Player 1 Tank
2 = Player 2 Tank
Space = Empty tile

Incorrect or malformed input will produce a readable error in input_errors.txt.

### Example Input File:
```
My Awesome Map
MaxSteps = 5000
NumShells = 16
Rows = 8
Cols = 10
##########
#        #
# 2      #
#        #
#    1   #
#     @  #
#   #    #
##########
```
This defines a 10x8 board with walls around the edges, tanks positioned, and a mine on the field.

---

## ⚡ Important Notes:
- Extra characters beyond the declared width per line are considered empty.
- If fewer lines or characters than expected are provided, the game fillful these lines/characters with spaces.

---

## 🖨️ Output Files

1. **output.txt**
Logs each round’s actions as a comma-separated list of tank actions.
Includes status changes: (ignored), (killed)

Ends with:
Last line in file will announce the winner in the following format: 
Player <X> won with <Y> tanks still alive 
Or: 
Tie, both players have zero tanks 
Or: 
Tie, reached max steps = <max_steps>, player 1 has <X> tanks, player 2 has <Y> tanks 
Or: 
Tie, both players have zero shells for <40> steps

   ### Example:
   ```
GetBattleInfo,RotateRight45,RotateRight45,RotateRight45
GetBattleInfo,RotateRight45,Shoot,Shoot (ignored)
GetBattleInfo,MoveForward,RotateRight45,Shoot
GetBattleInfo,RotateRight45,Shoot,Shoot (killed)
GetBattleInfo,RotateRight90,RotateRight90,RotateRight90
Player 2 won with 3 still alive
   ```

2. **input_errors.txt**
   - If any recoverable issues are found in the input file (e.g., multiple tanks for one player, wrong dimensions), they are recorded here.
---

## 🎮 Game Mechanics

- **Action Types**: Move forward, move backward (delayed), rotate (1/8 or 1/4), shoot.
- **Shooting**:
  - Each tank starts with 16 shells.
  - Shells move twice as fast as tanks.
  - A 4-turn cooldown applies after shooting.
- **Boundaries**: Toroidal world — tanks and shells wrap around board edges.
- **Collisions**:
  - Shell hits wall → Wall weakens; collapses after two hits.
  - Shell hits tank → Tank destroyed.
  - Shell hits shell → Both destroyed.
  - Tank steps on mine → Tank destroyed.
  - Tanks collide → Both destroyed.

---

## 🏆 Win & Draw Conditions

- **Victory by Elimination**: A player wins instantly when the opponent has no tanks remaining.
- **Mutual Destruction**: If both tanks are destroyed during the same turn, the game ends in a draw.
- **Ammunition Exhaustion**: If both tanks run out of shells, gameplay continues for 40 more turns (movement only). If neither tank is destroyed during this period, the game ends in a draw.
- **Turn-Based Resolution**: Win/loss conditions are evaluated after a full turn completes.
  - Example: If Player 1 is destroyed in the first half of a turn and Player 2 in the second half, the game results in a draw.

---

## 🧠 Algorithms

Each player is controlled by a deterministic algorithm:
- Full read access to the game board.
- Cannot modify the game state directly — they suggest actions to the `GameManager`.
- Player 1's tank chases Player 2 using BFS.
- Player 2's tank evades mines and shells to survive.

---

## 👾 Game Manager Responsibilities

- Orchestrates the game's progression by requesting moves from each player.
- Enforces game rules and turn order.
- Applies player actions (tank movement, shell firing).
- Manages collisions between tanks, shells, mines, and walls.
- Logs all actions and any invalid moves to the output file.

---

## 🏛️ Hierarchy Structure

```
GameManager
├── Player (1 & 2)
│   └── updateTankWithBattleInfo()
├── TankAlgorithm
│   └── getAction()
├── SatelliteView
├── BattleInfo
├── CollisionHandler
          └── Handles object collisions (shells, tanks, mines, walls)
└── GameBoard
            ├── Walls
            ├── Mines
            ├── Shells
            ├── Tanks
            └── Empty spaces
```

## Key Interfaces (per common/ folder):
TankAlgorithm.h
TankAlgorithmFactory.h
Player.h
PlayerFactory.h
SatelliteView.h
BattleInfo.h

---

## 🛠️ Building the Project

A standard **Makefile** is included.  
To compile the project, simply run:
```
make
```

---

## 🏃🏻‍♀️🏃🏼 Running the Simulation

After a successful build, run the program using:
```
./tanks_game <game_board_input_file>
```
Replace <game_board_input_file> with the path to your input text file.

---

# 🖼️ Visualizer (Bonus)
A Python utility to visualize gameplay using the input map and output.txt log.

## 🔧 Requirements
Python 3.x

pygame library:
```
pip install pygame
```
## 📁 Required Files
1. map.txt — formatted as described above
2. output.txt — generated by your simulation
3. images/ — directory containing:
      player1_up.png, player2_right.png, ...
      wall.png, mine.png, empty.png

▶️ Run
```
python visualizer.py
```
Optional:
Change FPS in the script for speed control
Enable toroidal edges by setting WRAP_EDGES = True

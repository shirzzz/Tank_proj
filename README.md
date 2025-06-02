# Tanks Game Simulator
**Advanced Topics in Programming — TAU Semester B 2025 — Assignment 1**

## 👩🏻‍💻👨🏼‍💻 Authors
- **Itai Lifshitz** —  
- **Shir Zadok** — 

---

## 📄 Overview
This project implements a deterministic simulation of a tank battle game on a 2D board for the "Advanced Topics in Programming" course at TAU.  
Each player controls a single tank, aiming to destroy the opponent while navigating walls, mines, and incoming artillery shells.

- **Player 1**: Actively chases Player 2 using a **BFS (Breadth-First Search)** algorithm.
- **Player 2**: Focuses on survival, evading shells and mines.

The game reads the board configuration from an input file, runs the battle based on predefined algorithms, and outputs the results into text files.

---

## 🔣 Input File Format

The input file is divided into two parts:
1. **Board Dimensions**:  
   - First line specifies the board size:
   ```
   width height
   ```
   - `width`: Number of columns (horizontal size).
   - `height`: Number of rows (vertical size).

2. **Board Layout**:  
   - Each subsequent line represents a row of the board, with each character corresponding to a specific object:
     - `1` → Player 1's Tank
     - `2` → Player 2's Tank
     - `#` → Wall
     - `@` → Mine
     - ` ` (space) → Empty space

### Example Input File:
```
10 8
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
- Extra characters beyond the declared width per line are ignored.
- If fewer lines or characters than expected are provided, the game will not start.
- Exactly **one** tank per player must exist; otherwise, the game will not run.

---

## 🖨️ Output Files

1. **output.txt**
   - Logs every action performed by both tanks, including invalid ("bad") moves.
   - At the end, states the game outcome (win/tie) and the destruction reason.

   ### Example:
   ```
   Tank 1 Actions:
   1. MOVE_FORWARD
   2. MOVE_FORWARD
   3. BAD_STEP
   4. BAD_STEP
   5. LOSE
   Reason: SHELL HIT ME

   Tank 2 Actions:
   1. ROTATE_EIGHTH_RIGHT
   2. MOVE_FORWARD
   3. ROTATE_EIGHTH_RIGHT
   4. SHOOT
   5. WIN
   Reason: I HIT OPPONENT WITH A SHELL
   ```

2. **input_errors.txt**
   - If any recoverable issues are found in the input file (e.g., multiple tanks for one player, wrong dimensions), they are recorded here.

   ### Example:
   ```
   Error: More than one tank for player 1
   Error: Line length shorter than expected
   ```

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
│
├── Player (x2)
│   ├── Strategy 1 (Chasing Opponent - BFS)
│   ├── Strategy 2 (Survival - Evading Shells & Mines)
│   └── Access to read-only game board
│
├── CollisionHandler
│   └── Handles object collisions (shells, tanks, mines, walls)
│
└── Game Board
    ├── Walls
    ├── Mines
    ├── Shells
    ├── Tanks
    └── Empty spaces
```

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

# Tanks Game Assignment 2 - To-Do List
Based on current project: https://github.com/shirzzz/Tank_proj

## 📁 Project Structure Review & Updates
- [x] ~Create project directory structure~ (Already exists)
- [x] ~~Set up common/ directory for shared interfaces~~ (Already exists)
- [ ] Review and update existing common/ interfaces for Assignment 2 requirements
- [ ] Update build system if needed (check current Makefile/CMake) //בסוף לסדר את זה

## 🔧 Common Interfaces Updates (Assignment 2)
- [ ] Update common/ActionRequest.h enum
  - [x] ~Basic actions already exist~ (MoveForward, MoveBackward, etc.)
  - [ ] *ADD: GetBattleInfo* action (NEW for Assignment 2)
  - [ ] Verify all rotation actions: RotateLeft45, RotateRight45
- [ ] Update common/TankAlgorithm.h abstract class
  - [x] ~getAction() method exists~
  - [ ] *ADD: updateBattleInfo(BattleInfo& info)* method (NEW)
- [ ] **CREATE: common/BattleInfo.h** abstract class (NEW)
- [ ] **CREATE: common/SatelliteView.h** abstract class (NEW)
  - [ ] Pure virtual getObjectAt(size_t x, size_t y) method
- [ ] **CREATE: common/Player.h** abstract class (NEW)
  - [ ] Constructor with parameters (player_index, x, y, max_steps, num_shells)
  - [ ] Pure virtual updateTankWithBattleInfo() method
- [ ] **CREATE: common/PlayerFactory.h** abstract class (NEW)
  - [ ] Pure virtual create() method
- [ ] **CREATE: common/TankAlgorithmFactory.h** abstract class (NEW)
  - [ ] Pure virtual create() method

## 📄 File Input/Output Updates
- [ ] *UPDATE existing map file parser* for Assignment 2 format
  - [ ] Verify Line 1: map name/description parsing
  - [ ] Verify Line 2: MaxSteps = <NUM> parsing
  - [ ] Verify Line 3: NumShells = <NUM> parsing  
  - [ ] Verify Line 4: Rows = <NUM> parsing
  - [ ] Verify Line 5: Cols = <NUM> parsing
  - [ ] Update Lines 6+: actual map data parsing
  - [ ] Handle spaces around '=' sign flexibility
  - [ ] Improve file validation and error messages
- [ ] *UPDATE map character recognition* (if needed)
  - [x] ~Basic characters likely exist~ (walls, tanks, etc.)
  - [ ] Verify mine (@) handling
  - [ ] Verify player tank numbering (can be any digit, including 0)
- [ ] *UPDATE output file writer* for Assignment 2 format
  - [ ] Modify to write single line per game round
  - [ ] Implement comma-separated tank actions in birth order
  - [ ] *ADD: (killed) and (ignored) annotations* 
  - [ ] *ADD: combined (ignored) (killed) cases*
  - [ ] Update final game result format to match specifications

## 🎮 Core Game Logic Updates
- [ ] *UPDATE existing GameManager class* for Assignment 2
  - [ ] *MODIFY constructor* to accept two factory methods (PlayerFactory & TankAlgorithmFactory)
  - [x] ~readBoard() method likely exists~
  - [x] ~run() method likely exists~
  - [ ] *UPDATE tank sequencing* to match Assignment 2 (top-left to bottom-right, row by row)
  - [ ] *ADD: Support for multiple tanks per player*
- [ ] *UPDATE existing game state management*
  - [x] ~Basic tank tracking likely exists~
  - [ ] Verify mines and walls tracking
  - [ ] *ADD: shells in flight tracking*
  - [ ] *ADD: remaining shells per tank tracking*
  - [ ] Verify game step counter implementation
- [ ] *IMPLEMENT NEW: SatelliteView concrete class*
  - [ ] Return '#' for walls
  - [ ] Return '1'/'2' for player tanks  
  - [ ] Return '%' for requesting tank
  - [ ] Return '@' for mines
  - [ ] Return '*' for artillery shells
  - [ ] Return ' ' for empty space
  - [ ] Return '&' for out-of-bounds
  - [ ] Handle shell-over-mine priority (show shell)

## 🚗 Tank Movement & Actions Updates
- [ ] *VERIFY/UPDATE existing tank movement logic*
  - [x] ~Basic movement likely implemented~
  - [ ] Ensure MoveForward/MoveBackward uses cannon direction
  - [ ] Verify collision detection with walls and tanks
  - [ ] Verify boundary checking
- [ ] *VERIFY/UPDATE existing tank rotation logic*
  - [x] ~Basic rotation likely implemented~
  - [ ] Ensure all rotations work: RotateLeft90, RotateRight90, RotateLeft45, RotateRight45
  - [ ] Verify cannon direction updates
  - [ ] Verify forward movement direction updates  
- [ ] *UPDATE existing shooting mechanics*
  - [x] ~Basic shooting likely implemented~
  - [ ] *ADD: Shell trajectory and flight tracking*
  - [ ] *ADD: Shell collision with walls/tanks*  
  - [ ] *ADD: Shell explosion on mines*
  - [ ] *ADD: Decrement shell count per tank*
- [ ] *IMPLEMENT NEW: GetBattleInfo action*
  - [ ] Call player's updateTankWithBattleInfo()  
  - [ ] Provide SatelliteView from previous round
  - [ ] Handle tank-player-tank communication sequence

## 👥 Player & Algorithm Implementation (NEW for Assignment 2)
- [ ] *IMPLEMENT NEW: Concrete Player class*
  - [ ] Store player index, battlefield dimensions  
  - [ ] Store max steps and max shells per tank
  - [ ] *COORDINATE between multiple player tanks* (new requirement)
  - [ ] Implement updateTankWithBattleInfo()
- [ ] *UPDATE existing TankAlgorithm* for Assignment 2
  - [x] ~Basic algorithm likely exists~
  - [ ] *ADD: updateBattleInfo() method implementation*
  - [ ] *ADD: Handle BattleInfo casting to concrete type*
  - [ ] *ENSURE: Deterministic algorithm* (no randomness - verify existing code)
- [ ] *IMPLEMENT NEW: Concrete BattleInfo class*
  - [ ] Data structure for battle information
  - [ ] Methods for algorithm-player communication
- [ ] *IMPLEMENT NEW: PlayerFactory concrete class*
  - [ ] Create Player instances with correct parameters
- [ ] *IMPLEMENT NEW: TankAlgorithmFactory concrete class*  
  - [ ] Create TankAlgorithm instances with player/tank indices
  - [ ] *MODIFY to work with existing algorithm implementation*

## 🎯 Game Rules & Win Conditions Updates
- [ ] *VERIFY/UPDATE existing game validation*
  - [ ] Check if player has no tanks (immediate loss)
  - [ ] Check if both players have no tanks (immediate tie)
- [ ] *VERIFY/UPDATE existing win condition checking*
  - [x] ~Basic win conditions likely exist~
  - [ ] Verify: Player wins when opponent has zero tanks
  - [ ] Verify: Tie when both players have zero tanks  
  - [ ] *ADD: Tie when max steps reached*
  - [ ] *ADD: Tie when both players have zero shells for 40 steps*
- [ ] *VERIFY/UPDATE tank death conditions*
  - [x] ~Basic death conditions likely exist~
  - [ ] Verify: Hit by shell
  - [ ] *ADD/VERIFY: Hit mine*
  - [ ] Verify: Track killed tanks properly

## 🔧 Main Function Updates
- [ ] *UPDATE existing main function* for Assignment 2
  - [x] ~Command line parsing likely exists~
  - [ ] *MODIFY: Create GameManager with factory methods* (new requirement)
    cpp
    GameManager game(MyPlayerFactory(), MyTankAlgorithmFactory());
    
  - [x] ~readBoard() call likely exists~
  - [x] ~run() call likely exists~
- [ ] *VERIFY existing command line handling*
  - [x] ~Input file argument likely handled~
  - [ ] Verify file not found error handling

## ✅ Testing & Validation Updates
- [ ] *UPDATE existing test files* for Assignment 2 format
  - [x] ~Test input files likely exist~
  - [ ] *CREATE 3 NEW test input files* matching Assignment 2 format
  - [ ] *GENERATE 3 corresponding output files* with new format
- [ ] *VERIFY existing functionality still works*
  - [ ] Test file format validation
  - [ ] Test edge cases (no tanks, invalid moves, etc.)  
- [ ] *NEW TESTING for Assignment 2 features*
  - [ ] Test GetBattleInfo action
  - [ ] Test multi-tank coordination
  - [ ] Test factory pattern implementation
  - [ ] *VERIFY deterministic behavior* (same input = same output)
  - [ ] Test action validation (ignored moves)
  - [ ] Test new win/tie conditions

## 🚀 Error Handling Updates
- [ ] *VERIFY existing error handling*
  - [x] ~Basic file handling likely exists~
  - [ ] Verify invalid input file handling
  - [ ] Verify out-of-bounds access prevention
  - [ ] *ADD: Invalid action handling for new actions*
- [ ] *REVIEW memory management* (Assignment 2 requirement)
  - [ ] *AUDIT: Ensure no new/delete usage* (strict requirement)
  - [ ] *VERIFY: Use smart pointers appropriately*
  - [ ] *VERIFY: Use stack variables when possible*
- [ ] Follow "exact same instructions as assignment 1" (verify consistency)

## 🎁 Bonus Features (Optional)
- [ ] Add logging system
- [ ] Add configuration file support
- [ ] Implement automatic testing with GTest
- [ ] Create visual simulation utility
- [ ] Extend game to support 2-9 players
- [ ] Create bonus.txt file describing implemented bonuses
- [ ] Create 4th input file for multi-player (if implemented)

## 📋 Code Quality & Requirements Review
- [ ] *AUDIT existing codebase* for Assignment 2 compliance
  - [ ] **VERIFY: No new and delete usage** (strict requirement)
  - [ ] *VERIFY: Appropriate std containers usage*
  - [ ] *VERIFY: Smart pointers usage* (prefer unique_ptr over shared_ptr)
  - [ ] *VERIFY: Stack variables used when possible*
- [ ] *ENSURE: Deterministic algorithm* (check existing implementation)
- [ ] *MAINTAIN: Exact API specifications* (no changes to provided interfaces)  
- [ ] *VERIFY: Proper file structure and naming*
- [ ] *CODE REVIEW: Compliance with Assignment 2 requirements*

## 📦 Submission Preparation
- [ ] Final code review and cleanup
- [ ] Test all 3 input/output file pairs
- [ ] Verify program runs with: tanks_game <game_board_input_file>
- [ ] Create submission package
- [ ] Include bonus.txt if applicable
- [ ] Verify submission follows guidelines

---

*Deadline: June 8th, 2025, 23:30*

*Key Notes:*
- API must be strictly followed (no modifications to provided interfaces)
- Algorithm must be deterministic (no randomness)
- Use smart pointers, avoid new/delete
- Tank birth order: top-left to bottom-right, row by row
- SatelliteView shows previous round state when GetBattleInfo is called

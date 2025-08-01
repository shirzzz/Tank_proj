CPP = g++

# Detect operating system and set flags accordingly
ifeq ($(OS),Windows_NT)
    # Windows - Remove AddressSanitizer (not reliable on MinGW)
    CPP_FLAGS = -std=c++20 -g -Wall -Wextra -Werror -pedantic
else
    # Linux/Mac - Keep AddressSanitizer
    CPP_FLAGS = -std=c++20 -g -Wall -Wextra -Werror -pedantic -fsanitize=address
endif

# Target executable (Windows adds .exe automatically)
TARGET = tanks_game

# Source files
SOURCES = BfsChaserShir.cpp Chased.cpp DirectionUtils.cpp GameBoard.cpp GameManager.cpp OurTester.cpp Shape.cpp Shell.cpp Tank.cpp Wall.cpp main.cpp MySatelliteView.cpp MyBattleInfo.cpp TankAlgorithm.cpp Player1.cpp Player2.cpp 

# Header files
HEADERS = ActionRequest.h BfsChaserShir.h CanonDirection.h CellType.h DestructionCause.h Chased.h DirectionUtils.h Empty.h GameBoard.h GameManager.h Mine.h OurTester.h Shape.h Shell.h Tank.h Wall.h Player1BattleInfo.h Player2BattleInfo.h PlayerFactory.h TankAlgorithmFactory.h SatelliteView.h TankAlgorithm.h BattleInfo.h MySatelliteView.h MyBattleInfo.h MyPlayerFactory.h MyTankAlgorithmFactory.h

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Detect operating system
ifeq ($(OS),Windows_NT)
    # Windows commands
    RM = del /Q
    RMDIR = rmdir /S /Q
    TARGET_EXT = .exe
    NULL_REDIRECT = 2>nul
else
    # Unix/Linux commands
    RM = rm -f
    RMDIR = rm -rf
    TARGET_EXT =
    NULL_REDIRECT = 2>/dev/null
endif

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJECTS)
	$(CPP) $(CPP_FLAGS) -o $(TARGET) $(OBJECTS)

# Pattern rule for object files (simplifies compilation)
%.o: %.cpp
	$(CPP) $(CPP_FLAGS) -c $< -o $@

# Individual dependencies (keeping your original dependencies)
BfsChaserShir.o: BfsChaserShir.cpp BfsChaserShir.h GameBoard.h DirectionUtils.h ActionType.h

Chased.o: Chased.cpp Chased.h DirectionUtils.h ActionType.h GameBoard.h Tank.h Shell.h

DirectionUtils.o: DirectionUtils.cpp DirectionUtils.h CanonDirection.h

GameBoard.o: GameBoard.cpp GameBoard.h CellType.h Tank.h Shell.h Shape.h Empty.h Wall.h Mine.h Chased.h BfsChaserShir.h

GameManager.o: GameManager.cpp GameManager.h GameBoard.h Tank.h Shell.h DestructionCause.h Chased.h ActionType.h CanonDirection.h CellType.h Empty.h

OurTester.o: OurTester.cpp OurTester.h GameBoard.h

Shape.o: Shape.cpp Shape.h

Shell.o: Shell.cpp Shell.h Shape.h CanonDirection.h

Tank.o: Tank.cpp Tank.h Shape.h CanonDirection.h ActionType.h DestructionCause.h

Wall.o: Wall.cpp Wall.h Shape.h

main.o: main.cpp GameBoard.h GameManager.h ActionType.h

Player1Algorithm.o: Player1Algorithm.cpp Player1Algorithm.h

Player2Algorithm.o: Player2Algorithm.cpp Player2Algorithm.h

MySatelliteView.o: MySatelliteView.cpp MySatelliteView.h

MyBattleInfo.o: MyBattleInfo.cpp MyBattleInfo.h

TankAlgorithm.o: TankAlgorithm.cpp common/TankAlgorithm.h MyBattleInfo.h DirectionUtils.h

MyPlayerFactory.o: MyPlayerFactory.cpp MyPlayerFactory.h GameBoard.h Player1.h Player2.h

MyTankAlgorithmFactory.o: MyTankAlgorithmFactory.cpp MyTankAlgorithmFactory.h GameBoard.h Player1.h Player2.h

Player1.o: Player1.cpp Player1.h MyBattleInfo.h MySatelliteView.h

Player2.o: Player2.cpp Player2.h MyBattleInfo.h MySatelliteView.h

clean:
ifeq ($(OS),Windows_NT)
	-del /Q *.o 2>nul
	-del /Q $(TARGET).exe 2>nul
	-del /Q tanks_game.exe 2>nul
	-del /Q log_file.txt 2>nul
	-del /Q input_errors.txt 2>nul
	-del /Q output_input_a.txt 2>nul
	-del /Q output_input_b.txt 2>nul
	-del /Q output_input_c.txt 2>nul
	@echo Clean completed.
else
	$(RM) $(OBJECTS) $(TARGET)
	$(RM) log_file.txt input_errors.txt
	$(RM) output_input_a.txt output_input_b.txt output_input_c.txt
endif

# Alternative clean that should work better on Windows
clean-force:
ifeq ($(OS),Windows_NT)
	@echo Cleaning build files...
	@if exist *.o del /Q *.o
	@if exist tanks_game.exe del /Q tanks_game.exe
	@if exist $(TARGET).exe del /Q $(TARGET).exe
	@if exist log_file.txt del /Q log_file.txt
	@if exist input_errors.txt del /Q input_errors.txt
	@if exist output_input_*.txt del /Q output_input_*.txt
	@echo Clean completed.
else
	$(RM) $(OBJECTS) $(TARGET)
	$(RM) log_file.txt input_errors.txt
	$(RM) output_input_a.txt output_input_b.txt output_input_c.txt
endif

# PowerShell clean (most reliable on Windows)
clean-ps:
	@powershell -Command "& { Get-ChildItem -Path . -Include *.o, tanks_game.exe, $(TARGET).exe, log_file.txt, input_errors.txt, output_input_*.txt -ErrorAction SilentlyContinue | Remove-Item -Force -ErrorAction SilentlyContinue; Write-Host 'Clean completed.' }"

# Help target
help:
	@echo Available targets:
	@echo   all         - Build the tanks_game executable
	@echo   clean       - Remove build files (cross-platform)
	@echo   clean-force - Force clean with existence checks
	@echo   clean-ps    - Remove build files using PowerShell (most reliable on Windows)
	@echo   help        - Show this help message

# Phony targets
.PHONY: all clean clean-force clean-ps help
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
SOURCES = Algorithm/BfsChaserShir.cpp Chased.cpp Algorithm/GameBoard.cpp GameManager/GameManager.cpp OurTester.cpp UserCommon/Shape.cpp UserCommon/Shell.cpp UserCommon/Tank.cpp UserCommon/Wall.cpp main.cpp UserCommon/MySatelliteView.cpp UserCommon/MyBattleInfo.cpp UserCommon/TankAlgorithm.cpp Algorithm/Player1.cpp Algorithm/Player2.cpp UserCommon/DirectionUtils.cpp

# Header files
HEADERS = common/ActionRequest.h Algorithm/BfsChaserShir.h UserCommon/CanonDirection.h UserCommon/CellType.h Algorithm/DestructionCause.h Chased.h UserCommon/DirectionUtils.h UserCommon/Empty.h Algorithm/GameBoard.h GameManager/GameManager.h UserCommon/Mine.h OurTester.h UserCommon/Shape.h UserCommon/Shell.h UserCommon/Tank.h UserCommon/Wall.h Algorithm/Player1BattleInfo.h Algorithm/Player2BattleInfo.h common/PlayerFactory.h common/TankAlgorithmFactory.h UserCommon/SatelliteView.h common/TankAlgorithm.h UserCommon/MySatelliteView.h UserCommon/MyBattleInfo.h UserCommon/MyPlayerFactory.h UserCommon/MyTankAlgorithmFactory.h

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
Algorithm/BfsChaserShir.o: Algorithm/BfsChaserShir.cpp Algorithm/BfsChaserShir.h Algorithm/GameBoard.h UserCommon/DirectionUtils.h common/ActionRequest.h

Chased.o: Chased.cpp Chased.h UserCommon/DirectionUtils.h common/ActionRequest.h Algorithm/GameBoard.h UserCommon/Tank.h UserCommon/Shell.h

Algorithm/GameBoard.o: Algorithm/GameBoard.cpp Algorithm/GameBoard.h UserCommon/CellType.h UserCommon/Tank.h UserCommon/Shell.h UserCommon/Shape.h UserCommon/Empty.h UserCommon/Wall.h UserCommon/Mine.h Chased.h Algorithm/BfsChaserShir.h

GameManager.o: GameManager.cpp GameManager.h Algorithm/GameBoard.h UserCommon/Tank.h UserCommon/Shell.h DestructionCause.h Chased.h UserCommon/CanonDirection.h UserCommon/CellType.h UserCommon/Empty.h

OurTester.o: OurTester.cpp OurTester.h Algorithm/GameBoard.h

UserCommon/Shape.o: UserCommon/Shape.cpp UserCommon/Shape.h

UserCommon/Shell.o: UserCommon/Shell.cpp UserCommon/Shell.h UserCommon/Shape.h UserCommon/CanonDirection.h

UserCommon/Tank.o: UserCommon/Tank.cpp UserCommon/Tank.h UserCommon/Shape.h UserCommon/CanonDirection.h common/ActionRequest.h DestructionCause.h

UserCommon/Wall.o: UserCommon/Wall.cpp UserCommon/Wall.h UserCommon/Shape.h

main.o: main.cpp Algorithm/GameBoard.h GameManager/GameManager.h

Player1Algorithm.o: Player1Algorithm.cpp Player1Algorithm.h

Player2Algorithm.o: Player2Algorithm.cpp Player2Algorithm.h

MySatelliteView.o: MySatelliteView.cpp MySatelliteView.h

MyBattleInfo.o: MyBattleInfo.cpp MyBattleInfo.h

UserCommon/TankAlgorithm.o: UserCommon/TankAlgorithm.cpp common/TankAlgorithm.h UserCommon/MyBattleInfo.h UserCommon/DirectionUtils.h

MyPlayerFactory.o: MyPlayerFactory.cpp MyPlayerFactory.h GameBoard.h Player1.h Player2.h

MyTankAlgorithmFactory.o: MyTankAlgorithmFactory.cpp MyTankAlgorithmFactory.h GameBoard.h Player1.h Player2.h

Algorithm/Player1.o: Algorithm/Player1.cpp Algorithm/Player1.h UserCommon/MyBattleInfo.h UserCommon/MySatelliteView.h

Algorithm/Player2.o: Algorithm/Player2.cpp Algorithm/Player2.h UserCommon/MyBattleInfo.h UserCommon/MySatelliteView.h

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
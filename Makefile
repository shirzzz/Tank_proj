CPP = g++
CPP_FLAGS = -std=c++20 -g -Wall -Wextra -Werror -pedantic

# Target executable
TARGET = tanks_game

# Source files
SOURCES = BfsChaserShir.cpp Chased.cpp DirectionUtils.cpp GameBoard.cpp GameManager.cpp OurTester.cpp Shape.cpp Shell.cpp Tank.cpp Wall.cpp main.cpp
HEADERS = ActionType.h BfsChaserShir.h CanonDirection.h CellType.h DestructionCause.h Chased.h DirectionUtils.h Empty.h GameBoard.h GameManager.h Mine.h OurTester.h Shape.h Shell.h Tank.h TankAI.h Wall.h

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJECTS)
	$(CPP) $(CPP_FLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files into object files

BfsChaserShir.o: BfsChaserShir.cpp BfsChaserShir.h GameBoard.h DirectionUtils.h ActionType.h
	$(CPP) $(CPP_FLAGS) -c BfsChaserShir.cpp -o BfsChaserShir.o

Chased.o: Chased.cpp Chased.h DirectionUtils.h ActionType.h GameBoard.h Tank.h Shell.h
	$(CPP) $(CPP_FLAGS) -c Chased.cpp -o Chased.o

DirectionUtils.o: DirectionUtils.cpp DirectionUtils.h CanonDirection.h
	$(CPP) $(CPP_FLAGS) -c DirectionUtils.cpp -o DirectionUtils.o

GameBoard.o: GameBoard.cpp GameBoard.h CellType.h Tank.h Shell.h Shape.h Empty.h Wall.h Mine.h Chased.h  BfsChaserShir.h
	$(CPP) $(CPP_FLAGS) -c GameBoard.cpp -o GameBoard.o

GameManager.o: GameManager.cpp GameManager.h GameBoard.h Tank.h Shell.h DestructionCause.h Chased.h ActionType.h CanonDirection.h CellType.h Empty.h
	$(CPP) $(CPP_FLAGS) -c GameManager.cpp -o GameManager.o

OurTester.o: OurTester.cpp OurTester.h GameBoard.h
	$(CPP) $(CPP_FLAGS) -c OurTester.cpp -o OurTester.o

Shape.o: Shape.cpp Shape.h
	$(CPP) $(CPP_FLAGS) -c Shape.cpp -o Shape.o

Shell.o: Shell.cpp Shell.h Shape.h CanonDirection.h
	$(CPP) $(CPP_FLAGS) -c Shell.cpp -o Shell.o

Tank.o: Tank.cpp Tank.h Shape.h CanonDirection.h ActionType.h DestructionCause.h
	$(CPP) $(CPP_FLAGS) -c Tank.cpp -o Tank.o

Wall.o: Wall.cpp Wall.h Shape.h
	$(CPP) $(CPP_FLAGS) -c Wall.cpp -o Wall.o

main.o: main.cpp GameBoard.h GameManager.h ActionType.h
	$(CPP) $(CPP_FLAGS) -c main.cpp -o main.o

# Clean up build files
clean:
		rm -f $(OBJECTS) $(TARGET)
		rm -rf log_file.txt
		rm -rf input_errors.txt
		rm -rf output_input_a.txt
		rm -rf output_input_b.txt
		rm -rf output_input_c.txt
# Phony targets
.PHONY: all clean
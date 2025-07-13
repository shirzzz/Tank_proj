CPP = g++
CPP_FLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -O2 -fPIC
LINK_FLAGS = -shared

# Output file name
TARGET = Algorithm_211466123_212399455.so

# Source files for this algorithm
#Itai in the end there supposed to be one algorithm and one player, but for now we have two players and two algorithms
SOURCES = Player_211466123_212399455.cpp TankAlgorithm_211466123_212399455.cpp BfsChaseShir.cpp GameBoard.cpp Player1.cpp Player2.cpp
# Include paths 
INCLUDES = -I../../common -I../../simulator -I../../GameManager -I../../UserCommon

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CPP) $(LINK_FLAGS) -o $@ $^

%.o: %.cpp
	$(CPP) $(CPP_FLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

CPP = g++
CPP_FLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -O2 -fPIC
LINK_FLAGS = -shared

# Output file name
TARGET = GameManager_211466123_212399455.so

# Source files for this game manager
SOURCES = GameManager_211466123_212399455.cpp
# Include paths 
INCLUDES = -I../../common -I../../simulator -I../../GameManager -I../../UserCommon -I../../Algorithm

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CPP) $(LINK_FLAGS) -o $@ $^

%.o: %.cpp
	$(CPP) $(CPP_FLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -fPIC -pthread
TARGET = simulator_211466123_212399455

OBJS = main.o Simulator.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -ldl -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Output binary
TARGET = ring_structure

# Source files
SRC = main_.cpp
OBJ = $(SRC:.cpp=.o)

# Default rule: Compile and link
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Clean build files
clean:
	rm -f $(OBJ) $(TARGET)

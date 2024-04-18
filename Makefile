# Compiler
CXX := g++
# Compiler flags
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -O3
# Source directory
SRC_DIR := src
# Build directory
BUILD_DIR := build
# Binary directory
BIN_DIR := bin

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
# Object files
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
# Executable name
EXEC := $(BIN_DIR)/BBchess

.PHONY: all clean

all: $(EXEC)

# Rule to compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to link object files into executable
$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	@$(RM) -r $(BUILD_DIR) $(BIN_DIR)

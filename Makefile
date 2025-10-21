# Compiler
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -O2 -Iinclude

# Directories
SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj

# Target executable
TARGET := $(BUILD_DIR)/main

# Find all source files recursively
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
# Compute object files path in OBJ_DIR, mirroring folder structure
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Default rule
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile each .cpp into its corresponding .o in OBJ_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the executable
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) eca_results/

# Phony targets
.PHONY: all clean run

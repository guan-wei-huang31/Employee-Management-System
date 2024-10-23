# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -locci

# Directories
SRC_DIR = src
BIN_DIR = bin

# Executable and source files
TARGET = $(BIN_DIR)/employee_management
SRC_FILES = $(SRC_DIR)/main.cpp $(SRC_DIR)/utility.cpp
HEADERS = $(SRC_DIR)/utility.h

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SRC_FILES) $(HEADERS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRC_FILES) -o $(TARGET) $(LDFLAGS)

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up the build artifacts
clean:
	rm -rf $(BIN_DIR)

# Phony targets (to avoid conflicts with files named 'all', 'run', or 'clean')
.PHONY: all run clean

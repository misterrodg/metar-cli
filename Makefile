# Compiler
CXX = g++
# Flags
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
# Directories
SRC_DIR = src
BUILD_DIR = build
# Source
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Object Files
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
# Result
EXE = $(BUILD_DIR)/metar

# Targets
all: $(EXE)

$(EXE): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

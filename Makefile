# Compiler and flags
CXX := g++
CXXFLAGS := \
-std=c++20 -Isrc\
-Idep/include -Ldep/lib -lglfw3dll -lz\
-DGL_API=0

# Directories
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

# Source files
CPP_SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/**/**/*.cpp) $(wildcard $(SRC_DIR)/**/**/**/*.cpp)
C_SRCS := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/**/**/*.c) $(wildcard $(SRC_DIR)/**/**/**/*.c)

# Combine all sources (including glad.c)
SRCS := $(CPP_SRCS) $(C_SRCS)

# Generate corresponding .o object files in the build/ directory
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SRCS)) \
        $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))

# Final executable
TARGET := $(BIN_DIR)/app.exe

# Default rule
all: $(TARGET)

# Link object files into the executable
$(TARGET): $(OBJS)
#   @mkdir $(BIN_DIR)
	$(CXX) $(OBJS) -o $@

# Compile C++ source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
#   @mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C source files (e.g., glad.c)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
#   @mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -x c -c $< -o $@

# Clean rule
clean:
	rm $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
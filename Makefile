# run on windows
# clear && if [ -f build/main.o ]; then rm build/main.o; fi && time make

# run on mac / linux
# clear && time make





# Flags
CXXFLAGS = -std=c++20
CFLAGS = 

# Directories
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin
DEP_DIR = dep/src
TARGET = $(BIN_DIR)/main

# Libraries
LIBS = -lz


# windows
CXX = g++
CC = gcc
LIBS += -Idep/include -Ldep/lib -lglfw3dll
GL_API = 0

# macos
# CXX = clang++
# CC = clang
# GLFW_INCLUDE_DIR = /opt/homebrew/opt/glfw/include
# GLFW_LIB_DIR = /opt/homebrew/opt/glfw/lib
# LIBS += -lglfw -L$(GLFW_LIB_DIR) -I$(GLFW_INCLUDE_DIR)
# GL_API = 1


# define GL api
LIBS += -DGL_API=$(GL_API)




# Source files (recursive)
CPP_SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
GLAD_SOURCE = $(DEP_DIR)/glad.c

# Object and dependency files
CPP_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CPP_SOURCES))
GLAD_OBJECT = $(OBJ_DIR)/glad.o
OBJECTS = $(CPP_OBJECTS) $(GLAD_OBJECT)
DEPS = $(OBJECTS:.o=.d)

# Default target
all: $(TARGET)

# Link executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LIBS)

# Pattern rule for compiling C++ source to object
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)

# Compile glad.c
$(OBJ_DIR)/glad.o: $(GLAD_SOURCE)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@if [ -f build/main.o ]; then rm build/main.o; fi

# Include generated dependency files
-include $(DEPS)

.PHONY: all clean

CC = g++

VERSION = 20

ERROR_FLAGS = \
# -Wall\
# -Wextra\
# -Werror\
# -pedantic

CORE_FILES = \
src/FastNoise/FastNoise.cpp\
src/shading/ambient_occlusion.cpp
DEP_FILES = dep/src/glad.c

FILES = $(CORE_FILES) $(DEP_FILES) src/main.cpp



# windows
LIBS = -Idep/include -Ldep/lib -lglfw3dll -lz

# macos (linux?)
# GLFW_INCLUDE_DIR = /opt/homebrew/opt/glfw/include
# GLFW_LIB_DIR = /opt/homebrew/opt/glfw/lib
# LIBS = -L$(GLFW_LIB_DIR) -lglfw -I$(GLFW_INCLUDE_DIR) -lz


all:
	$(CC) -std=c++$(VERSION) $(ERROR_FLAGS) $(FILES) -o main $(LIBS)
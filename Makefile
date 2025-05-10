CC = g++

VERSION = 20

ERROR_FLAGS = \
# -Wall\
# -Wextra\
# -Werror\
# -pedantic

CORE_FILES = \
src/voxel/logic/voxel.cpp\
src/core/maths.cpp\
src/FastNoise/FastNoise.cpp\
src/shading/ambient_occlusion.cpp
DEP_FILES = dep/src/glad.c

FILES = $(CORE_FILES) $(DEP_FILES) src/main.cpp



# windows
LIBS = -Idep/include -Ldep/lib -lglfw3dll -lz

# macos (linux?)
# LIBS = -Idep/include -Ldep/lib



all:
	$(CC) -std=c++$(VERSION) $(ERROR_FLAGS) $(FILES) -o main $(LIBS)
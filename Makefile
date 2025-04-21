CC = g++

VERSION = 20

ERROR_FLAGS = \
# -Wall\
# -Wextra\
# -Werror\
# -pedantic

CORE_FILES = \
src/renderer_3d/renderer_3d.cpp\
src/voxel/voxel_chunk.cpp\
src/voxel/voxel_mesher.cpp\
src/voxel/voxel_world.cpp\
src/voxel/voxel.cpp\
# src/file_parsers/obj_parser.cpp\
# src/core/maths.cpp src/core/mesh.cpp
DEP_FILES = dep/src/glad.c

FILES = $(CORE_FILES) $(DEP_FILES) src/main.cpp



# windows
LIBS = -Idep/include -Ldep/lib -lglfw3dll
# -I./glm

# macos (linux?)
# LIBS = -Idep/include -Ldep/lib -lglfw3dll



all:
	$(CC) -std=c++$(VERSION) $(ERROR_FLAGS) $(FILES) -o main $(LIBS)
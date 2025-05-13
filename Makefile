CC = g++

VERSION = 20

ERROR_FLAGS = \
# -Wall\
# -Wextra\
# -Werror\
# -pedantic

CORE_FILES = \
src/core/maths.cpp\
src/FastNoise/FastNoise.cpp\
src/shading/ambient_occlusion.cpp
DEP_FILES = dep/src/glad.c

FILES = $(CORE_FILES) $(DEP_FILES) src/main.cpp



# windows
LIBS = -Idep/include -Ldep/lib -lglfw3dll -lz

# macos (linux?)
# LIBS = -Idep/include -Ldep/lib -lz
# LIBS = -I/usr/local/include -lglfw -lz
# LIBS = -I/usr/local/include $(pkg-config --libs glfw3) $(pkg-config --static --libs glfw3) -lz


all:
	$(CC) -std=c++$(VERSION) $(ERROR_FLAGS) $(FILES) -o main $(LIBS)
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
# LIBS = -L$(GLFW_LIB_DIR) -lglfw -I$(GLFW_INCLUDE_DIR) -lz



# 0 = OpenGL
# 1 = Vulkan
GL_API = 0



all:
	$(CC) -std=c++$(VERSION) $(ERROR_FLAGS) -DGL_API=$(GL_API) $(FILES) -o main $(LIBS)
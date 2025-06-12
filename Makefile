CC_VERSION = 20

ERROR_FLAGS = \
# -Wall\
# -Wextra\
# -pedantic

CORE_FILES = \
src/FastNoise/FastNoise.cpp\
src/shading/ambient_occlusion.cpp
DEP_FILES = dep/src/glad.c

FILES = $(CORE_FILES) $(DEP_FILES) src/main.cpp




# GL_API
# 0 = OpenGL 4.6
# 1 = OpenGL 4.1
# 2 = Vulkan



# windows
CC = g++
LIBS = -Idep/include -Ldep/lib -lglfw3dll -lz
GL_API = 0

# macos
# CC = g++
# GLFW_INCLUDE_DIR = /opt/homebrew/opt/glfw/include
# GLFW_LIB_DIR = /opt/homebrew/opt/glfw/lib
# LIBS = -L$(GLFW_LIB_DIR) -lglfw -I$(GLFW_INCLUDE_DIR) -lz
# GL_API = 1

# linux
# CC = g++
# GLFW_INCLUDE_DIR = /opt/homebrew/opt/glfw/include
# GLFW_LIB_DIR = /opt/homebrew/opt/glfw/lib
# LIBS = -L$(GLFW_LIB_DIR) -lglfw -I$(GLFW_INCLUDE_DIR) -lz
# GL_API = 0



all:
	$(CC) -std=c++$(CC_VERSION) $(ERROR_FLAGS) -DGL_API=$(GL_API) $(FILES) -o main $(FLAGS) $(LIBS)
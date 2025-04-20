CC = gcc

ERROR_FLAGS = \
# -Wall\
# -Wextra\
# -Werror\
# -pedantic

CORE_FILES = \
src/renderer_3d/renderer_3d.c\
src/file_parsers/obj_parser.c\
src/core/darray.c src/core/hashmap.c src/core/maths.c src/core/mesh.c
DEP_FILES = dep/src/glad.c

FILES = $(CORE_FILES) $(DEP_FILES) src/main.c



# windows
LIBS = -Idep/include -Ldep/lib -lglfw3dll
# -I./glm

# macos (linux?)
# LIBS = -Idep/include -Ldep/lib -lglfw3dll



all:
	$(CC) $(ERROR_FLAGS) $(FILES) -o main $(LIBS)
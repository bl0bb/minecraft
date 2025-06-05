cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

# Find GLFW
find_package(glfw3 REQUIRED)

# Add glad manually
add_library(glad STATIC external/glad/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# Add your executable
add_executable(my_project src/main.cpp)

# Include glad + glfw
target_include_directories(my_project PRIVATE external/glad/include)

# Link everything
target_link_libraries(my_project PRIVATE glad glfw z GL X11 Xi Xrandr Xinerama Xcursor dl pthread)




# cmake_minimum_required(VERSION 3.10)
# project(MyProject)
# 
# set(CMAKE_CXX_STANDARD 17)
# 
# # Include GLAD
# add_subdirectory(external/glad)
# 
# # Add GLFW
# find_package(glfw3 REQUIRED)
# 
# # Include directories
# include_directories(include external/glad/include)
# 
# # Source files
# add_executable(my_project src/main.cpp)
# 
# # Link libraries
# target_link_libraries(my_project glfw glad GL X11 Xi Xrandr Xinerama Xcursor dl pthread)
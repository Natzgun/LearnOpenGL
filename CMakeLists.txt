cmake_minimum_required(VERSION 3.10)
project(helloworld)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

# glad
add_library(glad src/glad.c)
target_include_directories(glad PUBLIC include)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})

add_executable(OpenGL-project src/main.cc)

target_link_libraries(OpenGL-project glad ${GLFW_LIBRARIES} dl GL)

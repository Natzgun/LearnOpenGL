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

set(SOURCES
  src/Shader.cc
)

add_executable(OpenGL-project src/textures.cc ${SOURCES})

target_link_libraries(OpenGL-project glad ${GLFW_LIBRARIES} dl GL)

# Copy assets and shaders to the build directory on every build
# so that relative paths like "assets/texture.png" work regardless of cwd
add_custom_command(TARGET OpenGL-project POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:OpenGL-project>/assets)

add_custom_command(TARGET OpenGL-project POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/shaders $<TARGET_FILE_DIR:OpenGL-project>/shaders)

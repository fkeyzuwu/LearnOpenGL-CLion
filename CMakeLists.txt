cmake_minimum_required(VERSION 3.13)

project(LearnOpenGL-CLion)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Including GLFW
add_subdirectory(glfw)
include_directories(glfw/include)

# Including GLAD
include_directories(glad/include)
include_directories(glad/src)

# Including Assimp/glm
include_directories(${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*
        main.cpp
        glad/src/glad.c
        stb_image/stb_image.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# Linking GLFW and OGL
set(ASSIMP_LIBRARY "${CMAKE_SOURCE_DIR}/cmake-build-debug/libassimp-5.dll")
target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY} ${ASSIMP_LIBRARY})
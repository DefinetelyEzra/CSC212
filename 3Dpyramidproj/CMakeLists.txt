cmake_minimum_required(VERSION 3.10)

project(My3DProject)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files
set(SOURCES
    src/main.cpp
    # Add other source files here
    Texture.cpp
    shaderClass.cpp
    VAO.cpp
    VBO.cpp
    EBO.cpp
)

# Add the executable
add_executable(My3DProject ${SOURCES})

# Link OpenGL and GLFW
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(My3DProject OpenGL::GL GLFW GLEW)

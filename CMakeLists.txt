cmake_minimum_required(VERSION 3.15)
project(BlackHoleSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)

# Source files organized by module
set(SOURCES
    main.cpp
    ray.cpp
    physics.cpp
    rendering.cpp
)

# Header files
set(HEADERS
    constants.h
    ray.h
    physics.h
    rendering.h
)

# Create executable
add_executable(blackhole ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(blackhole PRIVATE
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link libraries
target_link_libraries(blackhole
    OpenGL::GL
    GLEW::GLEW
    glfw
    glm::glm
)

# Platform-specific settings
if(APPLE)
    target_link_libraries(blackhole "-framework Cocoa" "-framework IOKit")
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(blackhole PRIVATE /W4)
else()
    target_compile_options(blackhole PRIVATE -Wall -Wextra -Wpedantic)
endif()

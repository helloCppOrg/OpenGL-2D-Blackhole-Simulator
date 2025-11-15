cmake_minimum_required(VERSION 3.21)

project(blackhole)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

# Dependencies to link
set(DEPS glfw GLEW::GLEW glm::glm OpenGL::GL)

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

# Include directories (optional - modern imported targets handle this automatically)
target_include_directories(blackhole PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries
target_link_libraries(blackhole PRIVATE ${DEPS})
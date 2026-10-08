cmake_minimum_required(VERSION 3.10)
project(GLFWTest)

set(CMAKE_CXX_STANDARD 17)
set(SHADER_SOURCE_DIR "${CMAKE_SOURCE_DIR}/Shaders")
set(SHADER_BUILD_DIR "${CMAKE_BINARY_DIR}/Shaders")

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE OpenGL::GL GLEW::GLEW glfw)


file(REMOVE_RECURSE ${SHADER_BUILD_DIR})
file(MAKE_DIRECTORY ${SHADER_BUILD_DIR})

# Copy shaders
file(GLOB SHADER_FILES "${SHADER_SOURCE_DIR}/*.glsl")
foreach(SHADER_FILE ${SHADER_FILES})
    configure_file(${SHADER_FILE} ${SHADER_BUILD_DIR}/ COPYONLY)
endforeach()
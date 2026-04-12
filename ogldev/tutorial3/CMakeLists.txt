cmake_minimum_required(VERSION 3.15)

set(APP_NAME main)

# Project name and language
project(${APP_NAME} VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${APP_NAME} src/main.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLEW REQUIRED glew) # pkg-config --libs glew

target_include_directories(${APP_NAME} PRIVATE ${GLEW_INCLUDE_DIRS})
target_compile_definitions(${APP_NAME} PRIVATE GLEW_STATIC)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(${APP_NAME} PRIVATE
    OpenGL::GL
    GLUT::GLUT
    ${GLEW_LIBRARIES}
)

cmake_minimum_required(VERSION 3.5)

# We are building a C Game
set(CMAKE_C_STANDARD 99)
project(game C)
# set(CMAKE_CXX_STANDARD 20)
# project(game CXX)

# Download Raylib
# include(FetchContent)
# FetchContent_Declare(
#     raylib
#     GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
#     GIT_TAG        "master"
# )
# FetchContent_MakeAvailable(raylib)

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
  endif()
endif()


add_executable(${PROJECT_NAME} game.c)
set(raylib_VERBOSE 1)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

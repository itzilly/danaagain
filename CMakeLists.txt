cmake_minimum_required(VERSION 3.16)
project(GoofyNetworkingGame)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/4.5.0.tar.gz
)
FetchContent_MakeAvailable(raylib)

# Avoid building the examples and games from raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

add_subdirectory(networking)
add_subdirectory(engine)
add_subdirectory(client)
add_subdirectory(server)

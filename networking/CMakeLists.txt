project(NetworkingLib)

add_library(networking STATIC
    net_common.cpp
)

target_include_directories(networking PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${raylib_SOURCE_DIR}/src
)

target_include_directories(networking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
project(GameEngineLib)

add_library(engine STATIC
    engine.cpp
)

target_include_directories(engine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${raylib_SOURCE_DIR}/src
)

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
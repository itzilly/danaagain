project(Server)

add_executable(server
    main.cpp
)

target_include_directories(server PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../networking/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../engine/include
)

# Link against networking
target_link_libraries(server networking engine raylib winmm)

# For Windows, link against additional libraries if necessary
if (WIN32)
    target_link_libraries(server ws2_32)
endif()
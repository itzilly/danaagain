cmake_minimum_required(VERSION 3.16)
project(Client)

set(FETCHCONTENT_QUIET OFF)
include(FetchContent)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_SHALLOW TRUE
        GIT_TAG master
)
FetchContent_MakeAvailable(imgui)

# Build ImGui as a static library
add_library(ImGui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)
target_include_directories(ImGui PUBLIC ${imgui_SOURCE_DIR})

# rlImGui
FetchContent_Declare(
        rlImGui
        GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
        GIT_SHALLOW TRUE
        GIT_TAG main
)
FetchContent_MakeAvailable(rlImGui)

# Setup rlImGui to work with Raylib
add_library(rlImGui STATIC ${rlimgui_SOURCE_DIR}/rlImGui.cpp)
target_include_directories(rlImGui PUBLIC ${rlimgui_SOURCE_DIR} ${imgui_SOURCE_DIR})
target_link_libraries(rlImGui PRIVATE raylib ImGui)

add_executable(client
    main.cpp
    game.cpp
    net_client.cpp
)

target_include_directories(client PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../networking/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../engine/include
)

# Link against networking and engine
target_link_libraries(client raylib networking engine rlImGui ImGui)

# For Windows, link against additional libraries if necessary
if (WIN32)
    target_link_libraries(client ws2_32)
endif()

# Set the source and destination directories
set(ASSETS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../assets)
set(ASSETS_DST_DIR ${CMAKE_BINARY_DIR}/client)

# Custom script to handle the copy with error reporting
file(WRITE ${CMAKE_BINARY_DIR}/copy_assets.cmake
"
if(EXISTS \"${ASSETS_SRC_DIR}\")
    file(COPY \"${ASSETS_SRC_DIR}\" DESTINATION \"${ASSETS_DST_DIR}\")
    if(NOT EXISTS \"${ASSETS_DST_DIR}\")
        message(WARNING \"Failed to copy assets from ${ASSETS_SRC_DIR} to ${ASSETS_DST_DIR}\")
    endif()
else()
    message(WARNING \"Assets directory does not exist: ${ASSETS_SRC_DIR}\")
endif()
")

# Post-build command to run the custom script
add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/copy_assets.cmake
    COMMENT "Copying assets to output directory"
)

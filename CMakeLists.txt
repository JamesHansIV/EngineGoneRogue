cmake_minimum_required(VERSION 3.10)

project(Engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

OPTION(EDITOR "Enable Editor" OFF)
OPTION(EDITOR "Enable Debug Messages" OFF)

if(EDITOR)
    ADD_DEFINITIONS(-DEDITOR=1)
else()
    ADD_DEFINITIONS(-DEDITOR=0)
endif(EDITOR)
unset(EDITOR CACHE)

if(DEBUG_MESSAGES)
    ADD_DEFINITIONS(-DDEBUG_MESSAGES=1)
else()
    ADD_DEFINITIONS(-DDEBUG_MESSAGES=0)
endif(DEBUG_MESSAGES)
unset(DEBUG_MESSAGES CACHE)

file(GLOB_RECURSE engineSrc CONFIGURE_DEPENDS
    "src/*.cpp"
    "vendor/imgui/backends/imgui_impl_sdlrenderer2.cpp"
    "vendor/imgui/backends/imgui_impl_sdl2.cpp"
)

add_executable(Engine ${engineSrc})

target_include_directories(Engine PUBLIC "${CMAKE_SOURCE_DIR}/vendor/imgui")
target_include_directories(Engine PUBLIC "${CMAKE_SOURCE_DIR}/vendor/tinyxml2/tinyxml2")
target_include_directories(Engine PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_include_directories(Engine PUBLIC "${CMAKE_SOURCE_DIR}/src")

add_subdirectory(vendor/imgui)
add_subdirectory(vendor/tinyxml2/tinyxml2)


set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

target_link_libraries(Engine imgui tinyxml2)
target_link_libraries(Engine "${CMAKE_SOURCE_DIR}/lib/SDL2/libSDL2-2.0.0.dylib")
target_link_libraries(Engine "${CMAKE_SOURCE_DIR}/lib/SDL2_image/libSDL2_image-2.0.0.dylib")
target_link_libraries(Engine "${CMAKE_SOURCE_DIR}/lib/SDL2_mixer/libSDL2_mixer-2.0.0.dylib")
target_link_libraries(Engine "${CMAKE_SOURCE_DIR}/lib/SDL2_ttf/libSDL2_ttf-2.0.0.dylib")

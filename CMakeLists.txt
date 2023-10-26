cmake_minimum_required(VERSION 3.10)

project(Engine)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE engineSrc CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/Components/*.cpp"
    "src/Objects/*.cpp"
    "src/Core/*.cpp"
    "src/Events/*.cpp"
    "src/Graphics/*.cpp"
    "src/GUI/*.cpp"
    "vendor/imgui/backends/imgui_impl_sdlrenderer2.cpp"
    "vendor/imgui/backends/imgui_impl_sdl2.cpp"
)

add_executable(Engine ${engineSrc})

target_include_directories(Engine PUBLIC "${CMAKE_SOURCE_DIR}/vendor/imgui")
target_include_directories(Engine PUBLIC "${CMAKE_SOURCE_DIR}/include")

add_subdirectory(vendor/imgui)

target_link_libraries(Engine imgui)
target_link_libraries(Engine "${CMAKE_SOURCE_DIR}/lib/SDL2/libSDL2-2.0.0.dylib")
target_link_libraries(Engine "${CMAKE_SOURCE_DIR}/lib/SDL2_image/libSDL2_image-2.0.0.dylib")
target_link_libraries(Engine "${CMAKE_SOURCE_DIR}/lib/SDL2_mixer/libSDL2_mixer-2.0.0.dylib")
target_link_libraries(Engine "${CMAKE_SOURCE_DIR}/lib/SDL2_ttf/libSDL2_ttf-2.0.0.dylib")
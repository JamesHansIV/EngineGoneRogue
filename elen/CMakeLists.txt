cmake_minimum_required(VERSION 3.10)

project(Engine)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE engineSrc CONFIGURE_DEPENDS
    "srcc/main.cpp"
    "srcc/Animation/*.cpp"
    "srcc/Character/*.cpp"
    "srcc/Core/*.cpp"
    "srcc/Events/*.cpp"
    "srcc/Graphics/*.cpp"
    "srcc/GUI/*.cpp"
    "srcc/Object/*.cpp"
    "srcc/Physics/*.cpp"
)

add_executable(Engine ${engineSrc})

target_include_directories(Engine PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(Engine "${CMAKE_SOURCE_DIR}/lib/SDL2/libSDL2-2.0.0.dylib")
target_link_libraries(Engine "${CMAKE_SOURCE_DIR}/lib/SDL2_image/libSDL2_image-2.0.0.dylib")
target_link_libraries(Engine "${CMAKE_SOURCE_DIR}/lib/SDL2_mixer/libSDL2_mixer-2.0.0.dylib")
target_link_libraries(Engine "${CMAKE_SOURCE_DIR}/lib/SDL2_ttf/libSDL2_ttf-2.0.0.dylib")



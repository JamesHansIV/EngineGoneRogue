SRC_DIR = srcc
CORE_DIR = srcc/Core
GRAPHICS_DIR = srcc/Graphics
ANIMATION_DIR = srcc/Animation
CHARACTER_DIR = srcc/Character
PHYSICS_DIR = srcc/Physics
BUILD_DIR = build/debug
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(CORE_DIR)/*.cpp) $(wildcard $(GRAPHICS_DIR)/*.cpp) $(wildcard $(ANIMATION_DIR)/*.cpp) $(wildcard $(CHARACTER_DIR)/*.cpp) $(wildcard $(PHYSICS_DIR)/*.cpp) 
OBJ_NAME = play 
INCLUDE_PATHS = -Iinclude
LIBRARY_PATHS = -L/opt/homebrew/Cellar/sdl2/2.28.3/lib/ -L/opt/homebrew/Cellar/sdl2_image/2.6.3_2/lib -L/opt/homebrew/Cellar/sdl2_ttf/2.20.2/lib -L/opt/homebrew/Cellar/sdl2_mixer/2.6.3_1/lib
COMPILER_FLAGS = -std=c++11 -Wall -O0 -g -arch arm64
LINKER_FLAGS = -lsdl2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)
	
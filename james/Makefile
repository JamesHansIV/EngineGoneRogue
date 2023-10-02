SRC_DIR = src
BUILD_DIR = build/debug
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_NAME = play
INCLUDE_PATHS = -Iinclude
LIBRARY_PATHS = -L/opt/homebrew/Cellar/sdl2/2.28.3/lib/ -L/opt/homebrew/Cellar/sdl2_image/2.6.3_2/lib
COMPILER_FLAGS = -std=c++11 -Wall -O0 -g -arch arm64
LINKER_FLAGS = -lsdl2 -lSDL2_image

all: t1 t2 t3 t4 t5 t6

t1: 
	$(CC) $(COMPILER_FlAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) src/t1.cpp -o $(BUILD_DIR)/t1

t2: 
	$(CC) $(COMPILER_FlAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) src/t2.cpp -o $(BUILD_DIR)/t2

t3: 	
	$(CC) $(COMPILER_FlAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) src/t3.cpp -o $(BUILD_DIR)/t3

t4: 	
	$(CC) $(COMPILER_FlAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) src/t4.cpp -o $(BUILD_DIR)/t4

t5: 	
	$(CC) $(COMPILER_FlAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) src/t5.cpp -o $(BUILD_DIR)/t5

t6: 	
	$(CC) $(COMPILER_FlAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) src/t6.cpp -o $(BUILD_DIR)/t6
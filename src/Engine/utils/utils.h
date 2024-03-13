#pragma once
#include <iostream>
#include "Engine/Events/Event.h"
#include "SDL2/SDL.h"

enum {
SCREEN_WIDTH = 1024,
SCREEN_HEIGHT = 768
};

enum {
LEVEL_WIDTH = 1280,
LEVEL_HEIGHT = 960
};

enum {
TILE_SIZE = 32
};

enum {
LOAD_PROJECT = 1
};

enum {
FILEPATH_LEN = 128,
LABEL_LEN = 32
};

#ifdef DEBUG_MESSAGES
#if DEBUG_MESSAGES == 0 
    #define SDL_Log(...)(void)0
#endif
#endif

struct Rect {
    float x;
    float y;
    int w;
    int h;
};

const int kHealthBarHeight = 5;

using CustomEventType = Uint32;

extern CustomEventType custom_event_type;

void PushNewEvent(EventType eventType, void* data1 = nullptr,
                  void* data2 = nullptr);

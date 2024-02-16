#pragma once
#include <iostream>
#include "Engine/Events/Event.h"
#include "SDL2/SDL.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 520

#define LEVEL_WIDTH 1280
#define LEVEL_HEIGHT 960

#define TILE_SIZE 32

#define LOAD_PROJECT 1

#define FILEPATH_LEN 128
#define LABEL_LEN 32

struct Rect {
    float x;
    float y;
    int w;
    int h;
};

const int HEALTH_BAR_HEIGHT = 5;

using CustomEventType = Uint32;

extern CustomEventType custom_event_type;

void PushNewEvent(EventType eventType, void* data1 = nullptr,
                  void* data2 = nullptr);

#pragma once
#include <iostream>
#include "Engine/Events/Event.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_stdinc.h"

enum { ScreenWidth = 1024, ScreenHeight = 768 };

enum { LevelWidth = 4096, LevelHeight = 3072 };

enum { TileSize = 32 };

enum { ShouldLoadProject = 1 };

enum { FilepathLen = 128, LabelLen = 32 };

// NOLINTBEGIN
#ifdef DEBUG_MESSAGES
#if DEBUG_MESSAGES == 0
#define SDL_Log(...) (void)0
#endif
#endif
// NOLINTEND

struct Rect {
    float x;
    float y;
    int w;
    int h;
};

const int kHealthBarHeight = 5;

static Uint32 object_counter = 0;

using CustomEventType = Uint32;

extern CustomEventType custom_event_type;

void PushNewEvent(EventType eventType, void* data1 = nullptr,
                  void* data2 = nullptr);

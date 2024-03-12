#include "AudioManager.h"
#include <cassert>

AudioManager::AudioManager() {
    // Load Music
    Mix_Music* music = nullptr;
    music = Mix_LoadMUS("../assets/music/running-background.wav");
    if (music == nullptr) {
        printf("Failed to load beat music! SDL_mixer Error: %s\n",
               Mix_GetError());
        assert(false);
    }
    m_music["running-background"] = music;
}

void AudioManager::PlayMusic(const MusicId& id, bool loop) {
    Mix_PlayMusic(m_music[id], loop ? -1 : 0);
}

AudioManager::~AudioManager() {
    for (auto& music : m_music) {
        Mix_FreeMusic(music.second);
    }
}

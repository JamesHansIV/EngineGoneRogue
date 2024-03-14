#include "AudioManager.h"
#include <cassert>
#include "SDL2/SDL_log.h"

using std::string;

std::vector<std::pair<string, string>> music_files = {
    {"pause-screen", "../assets/music/pause-screen.wav"},
    {"thinking-music", "../assets/music/thinking-music.wav"},
    {"beat", "../assets/music/beat.wav"},
    {"background-intense", "../assets/music/background-intense.wav"}};

AudioManager::AudioManager() {
    //Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
               Mix_GetError());
        assert(false);
    }

    for (auto& music : music_files) {
        LoadMusic(music.first, music.second);
    }
    SetMusicVolume(100);
}

void AudioManager::LoadMusic(const MusicId& id, const string& filename) {
    Mix_Music* music = nullptr;
    music = Mix_LoadMUS(filename.c_str());
    SDL_Log("Loading music: %s", filename.c_str());
    if (music == nullptr) {
        printf("Failed to load %s music! SDL_mixer Error: %s\n", id.c_str(),
               Mix_GetError());
        assert(false);
    }
    m_music[id] = music;
}

void AudioManager::SetMusicVolume(int volume) {
    if (m_is_music_muted) {
        m_music_volume = volume;
        return;
    }

    m_music_volume = volume;
    Mix_VolumeMusic(volume);
}

void AudioManager::PlayMusic(const MusicId& id, bool loop) {
    if (Mix_PlayingMusic() != 0) {
        SDL_Log("Music is already playing");
        return;
    }

    Mix_PlayMusic(m_music[id], loop ? -1 : 0);
    m_current_music_id = id;
}

void AudioManager::PlayMusicOverride(const MusicId& id, bool loop) {
    if (m_current_music_id == id) {
        return;
    }

    Mix_FadeOutMusic(250);
    Mix_FadeInMusic(m_music[id], loop ? -1 : 0, 250);
    m_current_music_id = id;
}

void AudioManager::ToggleMusic() {
    if (m_is_music_muted) {
        Mix_VolumeMusic(m_music_volume);
    } else {
        // Save the current volume
        m_music_volume = Mix_VolumeMusic(-1);
        Mix_VolumeMusic(0);
    }
    m_is_music_muted = !m_is_music_muted;
}

AudioManager::~AudioManager() {
    for (auto& music : m_music) {
        Mix_FreeMusic(music.second);
    }
}

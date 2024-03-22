#include "AudioManager.h"
#include <cassert>
#include "SDL2/SDL_log.h"

using std::string;

std::vector<std::pair<string, string>> music_files = {
    {"pause-screen", "../assets/music/pause-screen.wav"},
    {"beat", "../assets/music/beat.wav"},
    {"background-intense", "../assets/music/background-intense.wav"}};

std::vector<std::pair<string, string>> sound_files = {
    {"scratch", "../assets/sound/scratch.wav"},
    {"high", "../assets/sound/high.wav"},
    {"easter-egg", "../assets/music/easter-egg.wav"},
    {"thinking-music", "../assets/music/thinking-music.wav"},
    {"low", "../assets/sound/low.wav"}};

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

    for (auto& sound : sound_files) {
        LoadSound(sound.first, sound.second);
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

void AudioManager::MuteMusic() {
    if (m_is_music_muted) {
        return;
    }
    m_is_music_muted = true;
    m_music_volume = Mix_VolumeMusic(-1);
    Mix_VolumeMusic(0);
}

void AudioManager::LoadSound(const MusicId& id, const string& filename) {
    Mix_Chunk* sound = nullptr;
    sound = Mix_LoadWAV(filename.c_str());
    if (sound == nullptr) {
        printf("Failed to load %s sound! SDL_mixer Error: %s\n", id.c_str(),
               Mix_GetError());
        assert(false);
    }
    m_sound[id] = sound;
}

ChannelId AudioManager::PlaySound(const MusicId& id, int volume, int loops) {
    if (m_is_sound_muted) {
        return -1;
    }

    const ChannelId channel_id = Mix_PlayChannel(-1, m_sound[id], loops);
    Mix_Volume(channel_id, volume);
    return channel_id;
}

void AudioManager::PauseSound(ChannelId id) {
    Mix_Pause(id);
}

void AudioManager::ResumeSound(ChannelId id) {
    Mix_Resume(id);
}

void AudioManager::StopSound(ChannelId id) {
    Mix_HaltChannel(id);
}

void AudioManager::ToggleSound() {
    m_is_sound_muted = !m_is_sound_muted;
}

void AudioManager::EnableSound() {
    m_is_sound_muted = false;
}

void AudioManager::StopMusic() {
    Mix_HaltMusic();
}

void AudioManager::PauseMusic() {
    Mix_PauseMusic();
}

void AudioManager::ResumeMusic() {
    Mix_ResumeMusic();
}

AudioManager::~AudioManager() {
    for (auto& music : m_music) {
        Mix_FreeMusic(music.second);
        music.second = nullptr;
    }

    for (auto& sound : m_sound) {
        Mix_FreeChunk(sound.second);
        sound.second = nullptr;
    }

    Mix_Quit();
}

#pragma once

#include <string>
#include "SDL2_mixer/SDL_mixer.h"

using MusicId = std::string;

class AudioManager {
   public:
    AudioManager();
    ~AudioManager();
    void PlayMusicOverride(const MusicId& id, bool loop);
    void PlayMusic(const MusicId& id, bool loop);
    void PlaySound(const MusicId& id, bool loop);
    void StopMusic();
    void StopSound();
    void PauseMusic();
    void PauseSound();
    void ResumeMusic();
    void ResumeSound();
    static void SetMusicVolume(int volume);
    void SetSoundVolume(int volume);
    void MuteMusic();
    void MuteSound();
    void UnmuteMusic();
    void UnmuteSound();
    void Clean();

   private:
    std::unordered_map<MusicId, Mix_Music*> m_music;
    MusicId m_current_music_id;
    void LoadMusic(const MusicId&, const std::string& filename);
};

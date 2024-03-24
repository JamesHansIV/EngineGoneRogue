#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "SDL2_mixer/SDL_mixer.h"

using MusicId = std::string;
// ChannelId is the id of the channel that the sound is playing
// Can be -1 to select all channels
using ChannelId = int;

class AudioManager {
   public:
    AudioManager();
    ~AudioManager();
    void PlayMusicOverride(const MusicId& id, bool loop);
    void PlayMusic(const MusicId& id, bool loop);
    ChannelId PlaySound(const MusicId& id, int volume, int loops);
    static void StopMusic();
    static void StopSound(ChannelId id);
    static void PauseMusic();
    static void PauseSound(ChannelId id);
    static void ResumeMusic();
    static void ResumeSound(ChannelId id);
    void EnableSound();
    void MuteMusic();
    void SetMusicVolume(int volume);
    void SetSoundVolume(int volume);
    void ToggleMusic();
    void ToggleSound();
    void Clean();

   private:
    std::unordered_map<MusicId, Mix_Music*> m_music;
    std::unordered_map<MusicId, Mix_Chunk*> m_sound;
    MusicId m_current_music_id;
    bool m_is_music_muted;
    bool m_is_sound_muted;
    int m_music_volume;
    void LoadMusic(const MusicId&, const std::string& filename);
    void LoadSound(const MusicId&, const std::string& filename);
};

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <string>
#include <unordered_map>

class SoundSystem {
public:
    SoundSystem();
    ~SoundSystem();
    void LoadSound(const std::string& soundName, const char* filePath);
    void PlaySound(const std::string& soundName);

private:
    std::unordered_map<std::string, Mix_Music*> soundEffects;

    bool IsMP3(const char* filePath);
};

SoundSystem::SoundSystem() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    }

    int flags = MIX_INIT_MP3;
    if ((Mix_Init(flags) & flags) != flags) {
        fprintf(stderr, "Mix_Init Error: %s\n", Mix_GetError());
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Mix_OpenAudio Error: %s\n", Mix_GetError());
    }
}
SoundSystem::~SoundSystem() {
    for (auto& sound : soundEffects) {
        Mix_FreeMusic(sound.second);
    }
    soundEffects.clear();

    Mix_CloseAudio();
    Mix_Quit();
}

bool SoundSystem::IsMP3(const char* filePath) {
    std::string fileStr(filePath);
    std::string ext = fileStr.substr(fileStr.find_last_of(".") + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == "mp3";
}

void SoundSystem::LoadSound(const std::string& soundName, const char* filePath) {
    if (IsMP3(filePath)) {
        fprintf(stderr, "Filepath : %s\n", filePath);
        Mix_Music* soundEffect = Mix_LoadMUS(filePath);
        if (!soundEffect) {
            fprintf(stderr, "Failed to load sound effect: %s\n", Mix_GetError());
        } else {
            soundEffects[soundName] = soundEffect;
        }
    } else {
        fprintf(stderr, "Unsupported sound format: %s\n", filePath);
    }
}

void SoundSystem::PlaySound(const std::string& soundName) {
    if (soundEffects.count(soundName) > 0) {
        Mix_Music* soundEffect = soundEffects[soundName];
        if (soundName == "background") {
            Mix_PlayMusic(soundEffect, -1); // -1 for loop indefinitely
        } else {
            Mix_PlayMusic(soundEffect, 0); // 0 for no loop
        }
    } else {
        fprintf(stderr, "Sound not found: %s\n", soundName.c_str());
    }
}
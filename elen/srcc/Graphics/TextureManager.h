#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include <string>
#include <map>

class TextureManager{
    public:
        static TextureManager* GetInstance() 
        {
            return m_Instance = (m_Instance != nullptr) ? m_Instance : new TextureManager();
        }

        bool Load(std::string id, std::string filename);
        void Drop(std::string id);
        void Clean();
        void Draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void DrawFrame(std::string id, int x, int y, int width, int heigt, int row, int frame, SDL_RendererFlip flip);

    private:
        TextureManager(){};
        std::map<std::string, SDL_Texture*> m_TextureMap;
        static TextureManager* m_Instance;
};
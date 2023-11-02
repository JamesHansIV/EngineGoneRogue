#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "Texture.h"

#include <string>
#include <map>


class Renderer{
    public:
        static Renderer* GetInstance() 
        {
            return m_Instance = (m_Instance != nullptr) ? m_Instance : new Renderer();
        }

        void Init();

        SDL_Renderer* GetRenderer() { return m_Renderer; }
        void Render();
        void RenderClear();
        void Destroy();

        Texture* AddTexture(std::string id, std::string filename);
        Texture* AddTexture(std::string id, const char* filename);
        Texture* GetTexture(std::string id) { return m_TextureMap.find(id) != m_TextureMap.end() ? m_TextureMap[id] : nullptr; }
        void Drop(std::string id);
        void Clean();
        void Draw(std::string id, SDL_Rect& srcRect, SDL_Rect& destRect, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void Draw(std::string id, SDL_Rect& srcRect, SDL_Rect& destRect, double angle, const SDL_Point* center, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void Draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void Draw(std::string id, int x, int y, int width, int height, double angle, const SDL_Point* center, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, double angle, const SDL_Point* center, SDL_RendererFlip flip);

    private:
        Renderer() {}
        SDL_Renderer* m_Renderer;
        std::map<std::string, Texture*> m_TextureMap;
        static Renderer* m_Instance;
};
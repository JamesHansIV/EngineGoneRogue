#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "Texture.h"
#include "TileMap.h"

#include "utils.h"

#include <string>
#include <map>
#include <cassert>
#include <set>

class GameObject;

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
        TileMap* AddTileMap(std::string id, std::string filename, int tileSize, int rows, int cols);
        TileMap* AddTileMap(std::string id, const char* filename, int tileSize, int rows, int cols);
        Texture* GetTexture(std::string id) { return m_TextureMap.find(id) != m_TextureMap.end() ? m_TextureMap[id] : nullptr; }
        void Drop(std::string id);
        void Clean();
        void DrawLine(int x1, int y1, int x2, int y2);
        void Draw(std::string id, SDL_Rect& srcRect, SDL_Rect& destRect, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void Draw(std::string id, SDL_Rect& srcRect, SDL_Rect& destRect, double angle, const SDL_Point* center, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void Draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void Draw(std::string id, int x, int y, int width, int height, double angle, const SDL_Point* center, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, double angle, const SDL_Point* center, SDL_RendererFlip flip);

        void MoveCameraX(float x);
        void MoveCameraY(float y);

        void CenterCameraOnObject();
        void SetCameraTarget(GameObject* target) {
            cameraTarget = target;
        }

    private:
        Renderer() {}
        SDL_Renderer* m_Renderer;
        SDL_Rect m_Camera {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        std::map<std::string, Texture*> m_TextureMap;
        std::set<std::string> m_Filepaths;
        static Renderer* m_Instance;
        GameObject* cameraTarget;
};
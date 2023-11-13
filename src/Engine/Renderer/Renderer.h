#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "Texture.h"
#include "TileMap.h"

#include "Engine/utils/utils.h"

#include <string>
#include <map>
#include <cassert>
#include <set>

class GameObject;

class Renderer{
    public:
        static Renderer* GetInstance()
        {
            return m_instance = (m_instance != nullptr) ? m_instance : new Renderer();
        }

        void Init();

        SDL_Renderer* GetRenderer() { return m_Renderer; }
        std::vector<std::string>& GetTextureIDs() { return m_TextureIDs; }
        void Render();
        void RenderClear();
        void Destroy();

        Texture* AddTexture(const std::string& id, const std::string& filename);
        Texture* AddTexture(const std::string& id, const char* filename);
        TileMap* AddTileMap(const std::string& id, const std::string& filename, int tileSize, int rows, int cols);
        TileMap* AddTileMap(const std::string& id, const char* filename, int tileSize, int rows, int cols);
        Texture* GetTexture(const std::string& id) { return m_TextureMap.find(id) != m_TextureMap.end() ? m_TextureMap[id] : nullptr; }
        void Drop(const std::string& id);
        void Clean();
        void DrawLine(int x1, int y1, int x2, int y2);
        void DrawRect(SDL_Rect& rect);
        void Draw(const std::string& id, SDL_Rect& srcRect, SDL_Rect& dstRect, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void Draw(const std::string& id, SDL_Rect& srcRect, SDL_Rect& dstRect, double angle, const SDL_Point* center, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void Draw(const std::string& id, int x, int y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void Draw(const std::string& id, int x, int y, int width, int height, double angle, const SDL_Point* center, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void DrawFrame(const std::string& id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void DrawFrame(const std::string& id, int x, int y, int width, int height, int row, int frame, double angle, const SDL_Point* center, SDL_RendererFlip flip);

        void MoveCameraX(float x);
        void MoveCameraY(float y);

        void CenterCameraOnObject();
        void SetCameraTarget(GameObject* target) {
            m_CameraTarget = target;
        }

        int GetCameraX() const{return m_Camera.x;};
        int GetCameraY() const{return m_Camera.y;};

        void SaveTextures();

    private:
        Renderer() {}
        SDL_Renderer* m_Renderer;
        SDL_Rect m_Camera {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        std::map<std::string, Texture*> m_TextureMap;
        std::vector<std::string> m_TextureIDs;
        std::set<std::string> m_Filepaths;
        static Renderer* m_instance;
        GameObject* m_CameraTarget;
};

#include "../Core/Engine.h"
#include "TextureManager.h"

TextureManager* TextureManager::m_Instance = nullptr;

bool TextureManager::Load(std::string id, std::string filename)
{
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if(surface == nullptr)
    {
        SDL_Log("Failed to load texture %s, %s", filename.c_str(), SDL_GetError());
        return false;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surface);
    if(texture == nullptr)
    {
        SDL_Log("Failed to create texture from surface %s, %s", filename.c_str(), SDL_GetError());
        return false;
    }
    m_TextureMap[id] = texture;
    return true;
}

void TextureManager::Drop(std::string id)
{
    SDL_DestroyTexture(m_TextureMap[id]);
    m_TextureMap.erase(id);
}

void TextureManager::Clean()
{
    for (auto it = m_TextureMap.begin(); it != m_TextureMap.end(); ++it)
    {
        SDL_DestroyTexture(it->second);
    }
    m_TextureMap.clear();

    SDL_Log("Texture Manager cleaned");
}

void TextureManager::Draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip)
{
    SDL_Rect srcRect = {0,0, width, height};
    SDL_Rect dstRect = {x, y, width, height};
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
} 

void TextureManager::DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip){
    SDL_Rect srcRect = {width*frame, height*(row-1), width, height};
    SDL_Rect dstRect = {x, y, width, height};
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}

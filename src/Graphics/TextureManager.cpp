#include "../Core/Engine.h"
#include "TextureManager.h"

TextureManager* TextureManager::m_Instance = nullptr;

bool TextureManager::AddTexture(std::string id, std::string filename) {
    Texture* textureWrapper = new Texture(filename);
    m_TextureMap[id] = textureWrapper;
    return true;
}

void TextureManager::Drop(std::string id)
{
    delete m_TextureMap[id];
    m_TextureMap.erase(id);
}

void TextureManager::Clean()
{
    for (auto it = m_TextureMap.begin(); it != m_TextureMap.end(); ++it)
    {
        delete it->second;
    }
    m_TextureMap.clear();

    SDL_Log("Texture Manager cleaned");
}

void TextureManager::Draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip)
{
    SDL_Rect srcRect = {0,0, width, height};
    SDL_Rect dstRect = {x, y, width, height};
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::Draw(std::string id, int x, int y, int width, int height, double angle, const SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_Rect srcRect = {0,0, width, height};
    SDL_Rect dstRect = {x, y, width, height};
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, angle, center, flip);
}

void TextureManager::DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip){
    SDL_Rect srcRect = {width*frame, height*(row-1), width, height};
    SDL_Rect dstRect = {x, y, width, height};
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, double angle, const SDL_Point* center, SDL_RendererFlip flip){
    SDL_Rect srcRect = {width*frame, height*(row-1), width, height};
    SDL_Rect dstRect = {x, y, width, height};
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, angle, center, flip);
}
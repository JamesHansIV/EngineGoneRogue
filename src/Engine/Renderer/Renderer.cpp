#include "Renderer.h"
#include "Engine/Engine.h"

Renderer* Renderer::m_Instance = nullptr;


void Renderer::Init() {
    SDL_Window* window = Engine::GetInstance()->GetWindow();
    m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(m_Renderer == nullptr){
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
    }
}

void Renderer::Destroy() {
    Clean();
    SDL_DestroyRenderer(m_Renderer);
}

void Renderer::RenderClear() {
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
    SDL_RenderClear(m_Renderer);
}

void Renderer::Render() {
    SDL_RenderPresent(m_Renderer);
}

bool Renderer::AddTexture(std::string id, std::string filename) {
    Texture* textureWrapper = new Texture(filename);
    m_TextureMap[id] = textureWrapper;
    return true;
}

void Renderer::Drop(std::string id)
{
    delete m_TextureMap[id];
    m_TextureMap.erase(id);
}

void Renderer::Clean()
{
    for (auto it = m_TextureMap.begin(); it != m_TextureMap.end(); ++it)
    {
        delete it->second;
    }
    m_TextureMap.clear();

    SDL_Log("Texture Manager cleaned");
}

void Renderer::Draw(std::string id, SDL_Rect& srcRect, SDL_Rect& dstRect, SDL_RendererFlip flip)
{
    
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, 0, nullptr, flip);
}

void Renderer::Draw(std::string id, SDL_Rect& srcRect, SDL_Rect& dstRect, double angle, const SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, angle, center, flip);
}

void Renderer::Draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip)
{
    SDL_Rect srcRect = {0,0, width, height};
    SDL_Rect dstRect = {x, y, width, height};
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, 0, nullptr, flip);
}

void Renderer::Draw(std::string id, int x, int y, int width, int height, double angle, const SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_Rect srcRect = {0,0, width, height};
    SDL_Rect dstRect = {x, y, width, height};
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, angle, center, flip);
}

void Renderer::DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip){
    SDL_Rect srcRect = {width*frame, height*(row-1), width, height};
    SDL_Rect dstRect = {x, y, width, height};
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, 0, nullptr, flip);
}

void Renderer::DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, double angle, const SDL_Point* center, SDL_RendererFlip flip){
    SDL_Rect srcRect = {width*frame, height*(row-1), width, height};
    SDL_Rect dstRect = {x, y, width, height};
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, angle, center, flip);
}
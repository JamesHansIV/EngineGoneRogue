#include "Renderer.h"
#include "Apps/Application.h"

bool checkCollision(SDL_Rect& a, SDL_Rect& b) {
    if (a.x + a.w < b.x)
        return false;
    if (b.x + b.w < a.x)
        return false;

    if (a.y + a.h < b.y)
        return false;
    if (b.y + b.h < a.y)
        return false;

    return true;
}

Renderer* Renderer::m_Instance = nullptr;

void Renderer::Init() {
    SDL_Window* window = Application::Get()->GetWindow();
    m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(m_Renderer == nullptr){
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        assert(false);
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

Texture* Renderer::AddTexture(std::string id, std::string filename) {
    Texture* textureWrapper = new Texture(filename);
    m_TextureMap[id] = textureWrapper;
    return textureWrapper;
}

Texture* Renderer::AddTexture(std::string id, const char* filename) {
    SDL_Log("Texture filepath: %s", filename);
    Texture* textureWrapper = new Texture(filename);
    m_TextureMap[id] = textureWrapper;
    return textureWrapper;
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
    if (!checkCollision(dstRect, m_Camera))
        return;
    
    dstRect.x -= m_Camera.x;
    dstRect.y -= m_Camera.y;
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, 0, nullptr, flip);
}

void Renderer::Draw(std::string id, SDL_Rect& srcRect, SDL_Rect& dstRect, double angle, const SDL_Point* center, SDL_RendererFlip flip)
{
    if (!checkCollision(dstRect, m_Camera))
        return;
    
    dstRect.x -= m_Camera.x;
    dstRect.y -= m_Camera.y;
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, angle, center, flip);
}

void Renderer::Draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip)
{
    SDL_Rect srcRect = {0,0, width, height};
    SDL_Rect dstRect = {x, y, width, height};

    if (!checkCollision(dstRect, m_Camera))
        return;
    
    dstRect.x -= m_Camera.x;
    dstRect.y -= m_Camera.y;
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, 0, nullptr, flip);
}

void Renderer::Draw(std::string id, int x, int y, int width, int height, double angle, const SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_Rect srcRect = {0,0, width, height};
    SDL_Rect dstRect = {x, y, width, height};

    if (!checkCollision(dstRect, m_Camera))
        return;
    
    dstRect.x -= m_Camera.x;
    dstRect.y -= m_Camera.y;
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, angle, center, flip);
}

void Renderer::DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip){
    SDL_Rect srcRect = {width*frame, height*(row-1), width, height};
    SDL_Rect dstRect = {x, y, width, height};

    if (!checkCollision(dstRect, m_Camera))
        return;
    
    dstRect.x -= m_Camera.x;
    dstRect.y -= m_Camera.y;
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, 0, nullptr, flip);
}

void Renderer::DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, double angle, const SDL_Point* center, SDL_RendererFlip flip){
    SDL_Rect srcRect = {width*frame, height*(row-1), width, height};
    SDL_Rect dstRect = {x, y, width, height};

    if (!checkCollision(dstRect, m_Camera))
        return;
    
    dstRect.x -= m_Camera.x;
    dstRect.y -= m_Camera.y;
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, angle, center, flip);
}

void Renderer::MoveCameraX(float x) {
    m_Camera.x += x;
    if (m_Camera.x < 0)
        m_Camera.x = 0;
    if (m_Camera.x + m_Camera.w > LEVEL_WIDTH)
        m_Camera.x = LEVEL_WIDTH - m_Camera.w;
}

void Renderer::MoveCameraY(float y) {
    m_Camera.y += y;
    if (m_Camera.y < 0)
        m_Camera.y = 0;
    if (m_Camera.y + m_Camera.h > LEVEL_HEIGHT)
        m_Camera.y = LEVEL_HEIGHT - m_Camera.h;
}
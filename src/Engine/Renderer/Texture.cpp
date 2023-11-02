#include "Texture.h"
#include "Renderer.h"

Texture::Texture(const std::string& filename) {
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if(surface == nullptr)
    {
        SDL_Log("Failed to load texture %s, %s", filename.c_str(), SDL_GetError());
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::GetInstance()->GetRenderer(), surface);
    if(texture == nullptr)
    {
        SDL_Log("Failed to create texture from surface %s, %s", filename.c_str(), SDL_GetError());
    }
    m_Texture = texture;
    m_Width = surface->w;
    m_Height = surface->h;
    m_TileWidth = m_Width;
    m_TileHeight = m_Height;
}

Texture::~Texture() {
    SDL_DestroyTexture(m_Texture);
}
#include "Texture.h"
#include "Renderer.h"

Texture::Texture(const std::string& filename, std::string id) {
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if(surface == nullptr)
    {
        throw std::runtime_error("Failed to load texture");
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::GetInstance()->GetRenderer(), surface);
    if(texture == nullptr)
    {
        throw std::runtime_error("Failed to create texture from surface");
    }

    m_ID = id;
    m_Texture = texture;
    m_Width = surface->w;
    m_Height = surface->h;
    m_TileWidth = m_Width;
    m_TileHeight = m_Height;
    m_ObjectCount = 0;
}

Texture::Texture(const char* filename, std::string id) {
    SDL_Surface* surface = IMG_Load(filename);
    if(surface == nullptr)
    {
        throw std::runtime_error("Failed to load texture");
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::GetInstance()->GetRenderer(), surface);
    if(texture == nullptr)
    {
        throw std::runtime_error("Failed to load texture");
    }

    m_ID = id;
    m_Texture = texture;
    m_Width = surface->w;
    m_Height = surface->h;
    m_TileWidth = m_Width;
    m_TileHeight = m_Height;
    m_ObjectCount = 0;
}



Texture::~Texture() {
    SDL_DestroyTexture(m_Texture);
}
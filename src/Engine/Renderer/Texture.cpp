#include "Texture.h"

#include <utility>
#include "Renderer.h"

Texture::Texture(const std::string& filename, std::string id) {
    m_file_path = filename;
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if (surface == nullptr) {
        throw std::runtime_error("Failed to load texture");
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(
        Renderer::GetInstance()->GetRenderer(), surface);
    if (texture == nullptr) {
        throw std::runtime_error("Failed to create texture from surface");
    }

    m_surface = surface;
    m_id = std::move(id);
    m_texture = texture;
    m_width = surface->w;
    m_height = surface->h;
    m_object_count = 0;
}

Texture::Texture(const char* filename, std::string id) {
    m_file_path = filename;
    SDL_Surface* surface = IMG_Load(filename);
    if (surface == nullptr) {
        throw std::runtime_error("Failed to load texture");
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(
        Renderer::GetInstance()->GetRenderer(), surface);
    if (texture == nullptr) {
        throw std::runtime_error("Failed to load texture");
    }

    m_id = std::move(id);
    m_texture = texture;
    m_width = surface->w;
    m_height = surface->h;
    m_object_count = 0;
}

Texture::Texture(const std::string& text, SDL_Color text_color,
                 std::string id) {
    m_file_path = "";
    m_cur_text = text;
    SDL_Surface* surface = TTF_RenderUTF8_Solid(
        Renderer::GetInstance()->GetFont(), text.c_str(), text_color);
    if (surface == nullptr) {
        throw std::runtime_error("Failed to load texture");
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(
        Renderer::GetInstance()->GetRenderer(), surface);
    if (texture == nullptr) {
        throw std::runtime_error("Failed to load texture");
    }

    m_id = std::move(id);
    m_texture = texture;
    m_width = surface->w;
    m_height = surface->h;
    m_object_count = 0;
}

Texture::~Texture() {
    SDL_DestroyTexture(m_texture);
    SDL_FreeSurface(m_surface);
}

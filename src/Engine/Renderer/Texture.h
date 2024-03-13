#pragma once

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <string>

class Texture {
   public:
    Texture(const std::string& filename, std::string id);
    Texture(const char* filename, std::string id);
    Texture(const std::string& text, SDL_Color text_color, std::string id);
    virtual ~Texture();

    [[nodiscard]] std::string GetID() const { return m_id; }

    [[nodiscard]] std::string GetFilePath() const { return m_file_path; }

    [[nodiscard]] std::string GetText() const { return m_cur_text; }

    [[nodiscard]] int GetWidth() const { return m_width; }

    [[nodiscard]] int GetHeight() const { return m_height; }

    [[nodiscard]] int GetObjectCount() const { return m_object_count; }

    void IncObjectCount() { m_object_count++; }

    SDL_Texture* GetTexture() { return m_texture; }

   private:
    std::string m_id;
    std::string m_file_path;
    SDL_Texture* m_texture;
    int m_object_count;
    int m_width;
    int m_height;
    std::string m_cur_text;
};

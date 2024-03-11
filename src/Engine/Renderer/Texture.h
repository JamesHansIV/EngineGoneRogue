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

    std::string GetID() const { return m_id; }

    std::string GetFilePath() const { return m_file_path; }

    std::string GetText() const { return m_cur_text; }

    int GetWidth() const { return m_width; }

    int GetHeight() const { return m_height; }

    int GetObjectCount() const { return m_object_count; }

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

#include "Renderer.h"
#include <tinyxml2.h>
#include "Engine/Application/Application.h"
#include "Engine/Config/Config.h"
#include "Engine/Objects/GameObject.h"

bool CheckCollision(SDL_Rect& a, SDL_Rect& b) {
    if (a.x + a.w < b.x) {
        return false;
    }
    if (b.x + b.w < a.x) {
        return false;
    }

    if (a.y + a.h < b.y) {
        return false;
    }
    if (b.y + b.h < a.y) {
        return false;
    }

    return true;
}

Renderer* Renderer::m_instance = nullptr;

void Renderer::Init() {
    SDL_Window* window = Application::Get()->GetWindow();
    m_camera = {0, 0, Application::Get()->GetWindowWidth(),
                Application::Get()->GetWindowHeight()};
    SDL_Log("Window width: %d, Window height: %d",
            Application::Get()->GetWindowWidth(),
            Application::Get()->GetWindowHeight());

    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr) {
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        assert(false);
    }
    // TODO: This is a hack to scale the window up and makes everything appear bigger.
    // But it ruins the camera and HUD placement.
    //SDL_RenderSetScale(m_Renderer, 1.25, 1.25);
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    m_font = TTF_OpenFont(
        "../assets/fonts/josefin-sans/JosefinSans-Regular.ttf", 24);
    if (m_font == nullptr) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        assert(false);
    }
}

void Renderer::SetCameraSize(int width, int height) {
    m_camera = {m_camera.x, m_camera.y, width, height};
}

void Renderer::Destroy() {
    Clean();
    SDL_DestroyRenderer(m_renderer);
}

void Renderer::RenderClear() {
    SDL_SetRenderDrawColor(m_renderer, 8, 21, 38, 255);
    SDL_RenderClear(m_renderer);
}

void Renderer::Render() {
    SDL_RenderPresent(m_renderer);
    CenterCameraOnObject();
}

Texture* Renderer::AddTexture(const std::string& id,
                              const std::string& filename) {
    Texture* texture_wrapper = nullptr;
    try {
        if (m_filepaths.find(filename) != m_filepaths.end()) {
            throw std::runtime_error("Filepath already exists");
        }
        texture_wrapper = new Texture(filename, id);
        m_texture_map[id] = texture_wrapper;
        m_filepaths.insert(filename);
        m_texture_i_ds.push_back(id);

    } catch (std::runtime_error& err) {
        SDL_LogError(0, "%s", err.what());
    }
    return texture_wrapper;
}

Texture* Renderer::AddTexture(const std::string& id, const char* filename) {
    Texture* texture_wrapper = nullptr;
    try {
        if (m_filepaths.find(filename) != m_filepaths.end()) {
            throw std::runtime_error("Filepath already exists" +
                                     std::string(filename));
        }
        texture_wrapper = new Texture(filename, id);
        m_texture_map[id] = texture_wrapper;
        m_filepaths.insert(filename);
        m_texture_i_ds.push_back(id);

    } catch (std::runtime_error& err) {
        SDL_LogError(0, "%s", err.what());
    }
    return texture_wrapper;
}

Texture* Renderer::AddTextTexture(const std::string& id,
                                  const std::string& text,
                                  SDL_Color text_color) {
    Texture* texture_wrapper = nullptr;
    try {
        if (m_texture_map.find(id) != m_texture_map.end()) {
            Texture* cur_texture = m_texture_map[id];
            // Todo(): Should not be using filePath to compare with text
            if (cur_texture->GetText() == text) {
                return cur_texture;
            }
            delete m_texture_map[id];
        }

        texture_wrapper = new Texture(text, text_color, id);
        m_texture_map[id] = texture_wrapper;
        m_texture_i_ds.push_back(id);

    } catch (std::runtime_error& err) {
        SDL_LogError(0, "%s", err.what());
    }
    return texture_wrapper;
}

TileMap* Renderer::AddTileMap(const std::string& id,
                              const std::string& filename, int tileSize,
                              int rows, int cols) {
    TileMap* texture_wrapper = nullptr;
    try {
        if (m_filepaths.find(filename) != m_filepaths.end()) {
            throw std::runtime_error("Filepath already exists");
        }
        SDL_Log("Creating new tilemap");
        texture_wrapper = new TileMap(filename, id, tileSize, rows, cols);
        m_texture_map[id] = texture_wrapper;
        m_filepaths.insert(filename);
        m_texture_i_ds.push_back(id);

    } catch (std::runtime_error& err) {
        SDL_LogError(0, "%s", err.what());
    }
    return texture_wrapper;
}

TileMap* Renderer::AddTileMap(const std::string& id, const char* filename,
                              int tileSize, int rows, int cols) {
    TileMap* texture_wrapper = nullptr;
    try {
        if (m_filepaths.find(filename) != m_filepaths.end()) {
            throw std::runtime_error("Filepath already exists");
        }
        SDL_Log("Creating new tilemap");
        texture_wrapper = new TileMap(filename, id, tileSize, rows, cols);
        m_texture_map[id] = texture_wrapper;
        m_filepaths.insert(filename);
        m_texture_i_ds.push_back(id);

    } catch (std::runtime_error& err) {
        SDL_LogError(0, "%s", err.what());
    }
    return texture_wrapper;
}

void Renderer::Drop(const std::string& id) {
    delete m_texture_map[id];
    m_texture_map.erase(id);
}

void Renderer::Clean() {
    for (auto& it : m_texture_map) {
        delete it.second;
    }
    m_texture_map.clear();
    TTF_CloseFont(m_font);

    SDL_Log("Texture Manager cleaned");
}

void Renderer::DrawLine(int x1, int y1, int x2, int y2, DrawColor color) {
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(m_renderer, x1 - m_camera.x, y1 - m_camera.y,
                       x2 - m_camera.x, y2 - m_camera.y);
}

void Renderer::DrawRect(SDL_Rect& rect, DrawColor color, bool filled) {
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    if (!CheckCollision(rect, m_camera)) {
        return;
    }
    rect.x -= m_camera.x;
    rect.y -= m_camera.y;
    if (filled) {
        SDL_RenderFillRect(m_renderer, &rect);
    } else {
        SDL_RenderDrawRect(m_renderer, &rect);
    }
}

void Renderer::DrawRects(std::vector<SDL_Rect> rects, DrawColor color,
                         bool filled) {
    for (auto& rect : rects) {
        DrawRect(rect, color, filled);
    }
}

void Renderer::Draw(const std::string& id, SDL_Rect& srcRect, SDL_Rect& dstRect,
                    SDL_RendererFlip flip) {
    if (!CheckCollision(dstRect, m_camera)) {
        return;
    }

    dstRect.x -= m_camera.x;
    dstRect.y -= m_camera.y;
    SDL_RenderCopyEx(m_renderer, m_texture_map[id]->GetTexture(), &srcRect,
                     &dstRect, 0, nullptr, flip);
}

void Renderer::Draw(const std::string& id, SDL_Rect& srcRect, SDL_Rect& dstRect,
                    double angle, const SDL_Point* center,
                    SDL_RendererFlip flip) {
    if (!CheckCollision(dstRect, m_camera)) {
        return;
    }

    dstRect.x -= m_camera.x;
    dstRect.y -= m_camera.y;
    if (m_texture_map[id] == nullptr) {
        // SDL_LogError(0, "Texture '%s' is null", id.c_str());
        return;
    }
    SDL_RenderCopyEx(m_renderer, m_texture_map[id]->GetTexture(), &srcRect,
                     &dstRect, angle, center, flip);
}

void Renderer::Draw(const std::string& id, int x, int y, int width, int height,
                    SDL_RendererFlip flip) {
    SDL_Rect const src_rect = {0, 0, width, height};
    SDL_Rect dst_rect = {x, y, width, height};

    if (!CheckCollision(dst_rect, m_camera)) {
        return;
    }

    dst_rect.x -= m_camera.x;
    dst_rect.y -= m_camera.y;
    SDL_RenderCopyEx(m_renderer, m_texture_map[id]->GetTexture(), &src_rect,
                     &dst_rect, 0, nullptr, flip);
}

void Renderer::Draw(const std::string& id, int x, int y, int width, int height,
                    double angle, const SDL_Point* center,
                    SDL_RendererFlip flip) {
    SDL_Rect const src_rect = {0, 0, width, height};
    SDL_Rect dst_rect = {x, y, width, height};

    if (!CheckCollision(dst_rect, m_camera)) {
        return;
    }

    dst_rect.x -= m_camera.x;
    dst_rect.y -= m_camera.y;
    SDL_RenderCopyEx(m_renderer, m_texture_map[id]->GetTexture(), &src_rect,
                     &dst_rect, angle, center, flip);
}

void Renderer::DrawFrame(const std::string& id, int x, int y, int width,
                         int height, int row, int col, int frame,
                         SDL_RendererFlip flip) {
    SDL_Rect const src_rect = {width * (frame + col), height * (row), width,
                               height};
    // New sprite is small, has to be scaled larger to look right. Could cause trouble in the future.
    int const new_x = (x + width / 2) - (width / 2 * 2);
    int const new_y = (y + height / 2) - (height / 2 * 2);
    SDL_Rect dst_rect = {new_x, new_y, width * 2, height * 2};

    if (!CheckCollision(dst_rect, m_camera)) {
        return;
    }

    dst_rect.x -= m_camera.x;
    dst_rect.y -= m_camera.y;
    SDL_RenderCopyEx(m_renderer, m_texture_map[id]->GetTexture(), &src_rect,
                     &dst_rect, 0, nullptr, flip);
}

void Renderer::DrawFrame(const std::string& id, int x, int y, int width,
                         int height, int row, int col, int frame, double angle,
                         const SDL_Point* center, SDL_RendererFlip flip) {
    SDL_Rect const src_rect = {width * (frame + col), height * (row), width,
                               height};
    SDL_Rect dst_rect = {x, y, width, height};

    if (!CheckCollision(dst_rect, m_camera)) {
        return;
    }

    dst_rect.x -= m_camera.x;
    dst_rect.y -= m_camera.y;
    SDL_RenderCopyEx(m_renderer, m_texture_map[id]->GetTexture(), &src_rect,
                     &dst_rect, angle, center, flip);
}

void Renderer::CenterCameraOnObject() {
    if (m_camera_target == nullptr) {
        return;
    }
    int const target_x =
        m_camera_target->GetX() + m_camera_target->GetWidth() / 2;
    int const target_y =
        m_camera_target->GetY() + m_camera_target->GetHeight() / 2;

    m_camera.x = target_x - Application::Get()->GetWindowWidth() / 2;
    m_camera.y = target_y - Application::Get()->GetWindowHeight() / 2;
}

void Renderer::MoveCameraX(float x) {
    m_camera.x += x;
    if (m_camera.x < 0) {
        m_camera.x = 0;
    }
    if (m_camera.x + m_camera.w > LevelWidth) {
        m_camera.x = LevelWidth - m_camera.w;
    }
}

void Renderer::MoveCameraY(float y) {
    m_camera.y += y;
    if (m_camera.y < 0) {
        m_camera.y = 0;
    }
    if (m_camera.y + m_camera.h > LevelHeight) {
        m_camera.y = LevelHeight - m_camera.h;
    }
}

void Renderer::SaveTextures(const char* filepath) {
    if (SaveTextureMap(filepath, m_texture_map) != 0) {
        SDL_Log("failed to save textures");
    }
}

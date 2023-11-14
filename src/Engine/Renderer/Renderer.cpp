#include "Renderer.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/GameObject.h"
#include <tinyxml2.h>


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
    if(m_CameraTarget != nullptr) {
        CenterCameraOnObject();
    }
}

Texture* Renderer::AddTexture(const std::string& id, const std::string& filename) {
    Texture* texture_wrapper = nullptr;
    try {
        if (m_Filepaths.find(filename) != m_Filepaths.end()) {
            throw std::runtime_error("Filepath already exists");
        }
        texture_wrapper = new Texture(filename, id);
        m_TextureMap[id] = texture_wrapper;
        m_Filepaths.insert(filename);
        m_TextureIDs.push_back(id);

    } catch(std::runtime_error& err) {
        SDL_LogError(0, "%s", err.what());
    }
    return texture_wrapper;
}

Texture* Renderer::AddTexture(const std::string& id, const char* filename) {
    Texture* texture_wrapper = nullptr;
    try {
        if (m_Filepaths.find(filename) != m_Filepaths.end()) {
            throw std::runtime_error("Filepath already exists");
        }
        texture_wrapper = new Texture(filename, id);
        m_TextureMap[id] = texture_wrapper;
        m_Filepaths.insert(filename);
        m_TextureIDs.push_back(id);


    } catch(std::runtime_error& err) {
        SDL_LogError(0, "%s", err.what());
    }
    return texture_wrapper;
}

TileMap* Renderer::AddTileMap(const std::string& id, const std::string& filename, int tileSize, int rows, int cols) {
    TileMap* texture_wrapper = nullptr;
    try {
        if (m_Filepaths.find(filename) != m_Filepaths.end()) {
            throw std::runtime_error("Filepath already exists");
        }
        SDL_Log("Creating new tilemap");
        texture_wrapper = new TileMap(filename, id, tileSize, rows, cols);
        m_TextureMap[id] = texture_wrapper;
        m_Filepaths.insert(filename);
        m_TextureIDs.push_back(id);


    } catch(std::runtime_error& err) {
        SDL_LogError(0, "%s", err.what());
    }
    return texture_wrapper;
}

TileMap* Renderer::AddTileMap(const std::string& id, const char* filename, int tileSize, int rows, int cols) {
    TileMap* texture_wrapper = nullptr;
    try {
        if (m_Filepaths.find(filename) != m_Filepaths.end()) {
            throw std::runtime_error("Filepath already exists");
        }
        SDL_Log("Creating new tilemap");
        texture_wrapper = new TileMap(filename, id, tileSize, rows, cols);
        m_TextureMap[id] = texture_wrapper;
        m_Filepaths.insert(filename);
        m_TextureIDs.push_back(id);


    } catch(std::runtime_error& err) {
        SDL_LogError(0, "%s", err.what());
    }
    return texture_wrapper;
}

void Renderer::Drop(const std::string& id)
{
    delete m_TextureMap[id];
    m_TextureMap.erase(id);
}

void Renderer::Clean()
{
    for (auto & it : m_TextureMap)
    {
        delete it.second;
    }
    m_TextureMap.clear();

    SDL_Log("Texture Manager cleaned");
}

void Renderer::DrawLine(int x1, int y1, int x2, int y2) {
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 100);
    SDL_RenderDrawLine(m_Renderer, x1 - m_Camera.x, y1 - m_Camera.y, x2 - m_Camera.x, y2 - m_Camera.y);
}

void Renderer::DrawRect(SDL_Rect& rect) {
    SDL_SetRenderDrawColor(m_Renderer, 0, 150, 255, 255);
    if (!CheckCollision(rect, m_Camera)) {
        return;
    }
    rect.x -= m_Camera.x;
    rect.y -= m_Camera.y;
    SDL_RenderDrawRect(m_Renderer, &rect);
}

void Renderer::DrawRects(std::vector<SDL_Rect> rects) {
    for (auto& rect : rects) {
        DrawRect(rect);
    }
}

void Renderer::Draw(const std::string& id, SDL_Rect& srcRect, SDL_Rect& dstRect, SDL_RendererFlip flip)
{
    if (!CheckCollision(dstRect, m_Camera)) {
        return;
}

    dstRect.x -= m_Camera.x;
    dstRect.y -= m_Camera.y;
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, 0, nullptr, flip);
}

void Renderer::Draw(const std::string& id, SDL_Rect& srcRect, SDL_Rect& dstRect, double angle, const SDL_Point* center, SDL_RendererFlip flip)
{
    if (!CheckCollision(dstRect, m_Camera)) {
        return;
}

    dstRect.x -= m_Camera.x;
    dstRect.y -= m_Camera.y;
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &srcRect, &dstRect, angle, center, flip);
}

void Renderer::Draw(const std::string& id, int x, int y, int width, int height, SDL_RendererFlip flip)
{
    SDL_Rect const src_rect = {0,0, width, height};
    SDL_Rect dst_rect = {x, y, width, height};

    if (!CheckCollision(dst_rect, m_Camera)) {
        return;
}

    dst_rect.x -= m_Camera.x;
    dst_rect.y -= m_Camera.y;
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &src_rect, &dst_rect, 0, nullptr, flip);
}

void Renderer::Draw(const std::string& id, int x, int y, int width, int height, double angle, const SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_Rect const src_rect = {0,0, width, height};
    SDL_Rect dst_rect = {x, y, width, height};

    if (!CheckCollision(dst_rect, m_Camera)) {
        return;
}

    dst_rect.x -= m_Camera.x;
    dst_rect.y -= m_Camera.y;
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &src_rect, &dst_rect, angle, center, flip);
}

void Renderer::DrawFrame(const std::string& id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip){
  SDL_Rect const src_rect = {width*frame, height*(row-1), width, height};
  // New sprite is small, has to be scaled larger to look right. Could cause trouble in the future.
  int const new_x = (x + width/2) - (width/2 * 2);
  int const new_y = (y + height/2) - (height/2 * 2);
  SDL_Rect dst_rect = {new_x, new_y, width*2, height*2};

  if (!CheckCollision(dst_rect, m_Camera)) {
    return;
  }

  dst_rect.x -= m_Camera.x;
  dst_rect.y -= m_Camera.y;
  SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &src_rect, &dst_rect, 0, nullptr, flip);
}

void Renderer::DrawFrame(const std::string& id, int x, int y, int width, int height, int row, int frame, double angle, const SDL_Point* center, SDL_RendererFlip flip){
    SDL_Rect const src_rect = {width*frame, height*(row-1), width, height};
    SDL_Rect dst_rect = {x, y, width, height};

    if (!CheckCollision(dst_rect, m_Camera)) {
        return;
}

    dst_rect.x -= m_Camera.x;
    dst_rect.y -= m_Camera.y;
    SDL_RenderCopyEx(m_Renderer, m_TextureMap[id]->GetTexture(), &src_rect, &dst_rect, angle, center, flip);
}

void Renderer::CenterCameraOnObject() {

  int const target_x = m_CameraTarget->GetX() + m_CameraTarget->GetWidth() / 2;
  int const target_y = m_CameraTarget->GetY() + m_CameraTarget->GetHeight() / 2;

  m_Camera.x = target_x - SCREEN_WIDTH/2;
  m_Camera.y = target_y - SCREEN_HEIGHT/2;

}
void Renderer::MoveCameraX(float x) {
    m_Camera.x += x;
    if (m_Camera.x < 0) {
        m_Camera.x = 0;
}
    if (m_Camera.x + m_Camera.w > LEVEL_WIDTH) {
        m_Camera.x = LEVEL_WIDTH - m_Camera.w;
}
}

void Renderer::MoveCameraY(float y) {
    m_Camera.y += y;
    if (m_Camera.y < 0) {
        m_Camera.y = 0;
}
    if (m_Camera.y + m_Camera.h > LEVEL_HEIGHT) {
        m_Camera.y = LEVEL_HEIGHT - m_Camera.h;
}
}

void Renderer::SaveTextures() {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("Root");
    doc.InsertFirstChild(root);

    tinyxml2::XMLElement* texture;
    tinyxml2::XMLElement* id;
    tinyxml2::XMLElement* file_path;
    tinyxml2::XMLElement* type;

    tinyxml2::XMLElement* tile_size;
    tinyxml2::XMLElement* rows;
    tinyxml2::XMLElement* cols;


    for (auto & it : m_TextureMap) {
        texture = doc.NewElement("Texture");
        type = doc.NewElement("Type");
        id = doc.NewElement("ID");
        file_path = doc.NewElement("FilePath");

        auto* tile_map = dynamic_cast<TileMap*>(it.second);
        if (tile_map != nullptr) {
            SDL_Log("Saving tilemap");
            texture->SetAttribute("type", "tileMap");
            tile_size = doc.NewElement("TileSize");
            rows = doc.NewElement("Rows");
            cols = doc.NewElement("Cols");

            tile_size->SetText(tile_map->GetTileSize());
            rows->SetText(tile_map->GetRows());
            cols->SetText(tile_map->GetCols());

            texture->InsertEndChild(tile_size);
            texture->InsertEndChild(rows);
            texture->InsertEndChild(cols);
        } else {
            texture->SetAttribute("type", "base");
        }

        id->SetText(it.first.c_str());
        file_path->SetText(it.second->GetFilePath().c_str());

        texture->InsertEndChild(file_path);
        texture->InsertEndChild(id);

        root->InsertEndChild(texture);
    }

    char dst_path[128];
    sprintf(dst_path, "../assets/projects/%s/textures.xml", Application::Get()->GetProjectName().c_str());
    int const success = doc.SaveFile(dst_path);
    SDL_Log("Saving textures a success: %d", success);
}

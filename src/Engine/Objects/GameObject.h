#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
#include <utility>
#include "Engine/Animation/Animation.h"
#include "Engine/Physics/CollisionBox.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/TileMap.h"
#include "Engine/State/State.h"
#include "Health.h"
#include "IObject.h"

#include "Engine/utils/utils.h"

// enum ObjectCategory {
//     kNone = 0,
//     kCollider = 1 << 0,
//     kProjectile = 1 << 1,
//     kCharacter = 1 << 2,
//     kWeapon = 1 << 3
// };

class Properties {
   public:
    Properties(std::string textureID, TilePos tilePos, Rect dstRect,
               float rotation = 0.0F, std::string objectID = "",
               SDL_RendererFlip flip = SDL_FLIP_NONE) {
        TilePosition = tilePos;
        DstRect = dstRect;
        Flip = flip;
        Rotation = rotation;
        TextureID = std::move(textureID);
        ObjectID = std::move(objectID);
    }

    Properties(const Properties& prop) {
        TilePosition = prop.TilePosition;
        DstRect = prop.DstRect;
        Flip = prop.Flip;
        Rotation = prop.Rotation;
        TextureID = prop.TextureID;
        ObjectID = prop.ObjectID;
    }

    Properties(const Properties& prop, Rect rect, float rotation) {
        TilePosition = prop.TilePosition;
        DstRect = rect;
        Flip = prop.Flip;
        Rotation = rotation;
        TextureID = prop.TextureID;
        ObjectID = prop.ObjectID;
    }

    Properties& operator=(const Properties& prop) = default;

    std::string ObjectID;
    std::string TextureID;
    TilePos TilePosition;
    Rect DstRect;
    float Rotation;
    SDL_RendererFlip Flip;
};

class GameObject : public IObject {
   public:
    explicit GameObject(Properties& props)
        : m_texture_id(props.TextureID),
          m_current_tile_pos(props.TilePosition),
          m_dst_rect(props.DstRect),
          m_rotation(props.Rotation),
          m_flip(props.Flip),
          m_object_id(props.ObjectID),
          m_animation(nullptr),
          m_current_state(nullptr),
          m_marked_for_deletion(false) {}

    explicit GameObject(GameObject* rhs);
    virtual ~GameObject();

    void Draw() override;
    void Clean() override{};
    void Update(float dt) override;

    void DrawRect();
    void DrawAnimation();
    void ChangeState(State* state);

    virtual ObjectType GetObjectType() { return ObjectType::Base; }

    TilePos& GetTilePos() { return m_current_tile_pos; }

    void SetTilePos(TilePos tilePos) { m_current_tile_pos = tilePos; }

    void AddStillFrame(const std::string& id, TilePos tilePos);

    void SelectStillFrame(const std::string& id);

    Rect& GetDstRect() { return m_dst_rect; }

    float& GetX() { return m_dst_rect.x; }

    float& GetY() { return m_dst_rect.y; }

    void SetX(float x) { m_dst_rect.x = x; }

    void SetY(float y) { m_dst_rect.y = y; }

    void MoveX(float x) { m_dst_rect.x += x; }

    void MoveY(float y) { m_dst_rect.y += y; }

    [[nodiscard]] float GetMidPointX() const {
        return (m_dst_rect.x + static_cast<float>(m_dst_rect.w) / 2);
    };

    [[nodiscard]] float GetMidPointY() const {
        return (m_dst_rect.y + static_cast<float>(m_dst_rect.h) / 2);
    };

    int& GetWidth() { return m_dst_rect.w; }

    int& GetHeight() { return m_dst_rect.h; }

    void SetWidth(int width) { m_dst_rect.w = width; }

    void SetHeight(int height) { m_dst_rect.h = height; }

    float& GetRotation() { return m_rotation; }

    [[nodiscard]] float GetRadians() const { return m_rotation * (M_PI / 180); }

    void SetRotation(float rotation) { m_rotation = rotation; }

    std::string GetTextureID() { return m_texture_id; }

    void SetTextureID(std::string texture) { m_texture_id = std::move(texture); };

    std::string GetID() { return m_object_id; }

    void SetID(std::string id) { m_object_id = std::move(id); }

    SDL_RendererFlip GetFlip() { return m_flip; }

    void SetFlip(SDL_RendererFlip flip) { m_flip = flip; }

    Animation* GetAnimation() { return m_animation; }

    void SetAnimation(Animation* animation) { m_animation = animation; }

    State* GetCurrentState() { return m_current_state; }

    StateType GetStateType() { return m_current_state->GetType(); }

    [[nodiscard]] bool MarkedForDeletion() const { return m_marked_for_deletion; }

    void MarkForDeletion() { m_marked_for_deletion = true; }

   protected:
    TilePos m_current_tile_pos;
    std::unordered_map<std::string, TilePos> m_still_frames;
    Rect m_dst_rect;
    float m_rotation;
    std::string m_texture_id;
    std::string m_object_id;
    SDL_RendererFlip m_flip;
    Animation* m_animation;
    State* m_current_state;
    bool m_marked_for_deletion;
};

#pragma once

#include "IObject.h"
#include "Engine/Physics/Transform.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/TileMap.h"
#include "Health.h"

#include "Engine/Physics/Collider.h"
#include <SDL2/SDL.h>
#include <string>
#include <utility>


enum class ObjectType {
    kNone = 0,
    kBase, kProjectile, kPlayer
};

struct Rect {
    float x;
    float y;
    int w;
    int h;
};

struct Properties{
    public:
        Properties(
            std::string textureID,
            TilePos tilePos,
            Rect dstRect,
            float rotation = 0.0F,
            std::string objectID = "",
            SDL_RendererFlip flip = SDL_FLIP_NONE
        ){
            TilePosition = tilePos;
            DstRect = dstRect;
            Flip = flip;
            Rotation = rotation;
            TextureID = std::move(textureID);
            ObjectID = std::move(objectID);
        }
        std::string ObjectID;
        std::string TextureID;
        TilePos TilePosition;
        Rect DstRect;
        float Rotation;
        SDL_RendererFlip Flip;
};

class GameObject : public IObject {
    public:
        explicit GameObject(Properties& props): m_TextureID(props.TextureID),
            m_TilePos(props.TilePosition), m_DstRect(props.DstRect),
            m_Rotation(props.Rotation),
            m_Flip(props.Flip), m_ObjectID(props.ObjectID),
            m_Collider(nullptr) {

            m_Transform = new Transform(&m_DstRect.x, &m_DstRect.y);
        }

        explicit GameObject(GameObject* rhs) {
            m_TextureID = rhs->m_TextureID;
            m_TilePos = rhs->m_TilePos;
            m_DstRect = rhs->m_DstRect;
            m_Rotation = rhs->m_Rotation;
            m_Flip = rhs->m_Flip;
            m_ObjectID = rhs->m_ObjectID;

            m_Transform = new Transform(&m_DstRect.x, &m_DstRect.y);
        }
        virtual ~GameObject() = default;

        virtual ObjectType GetObjectType() { return ObjectType::kBase; }

        void Draw() override {
            SDL_Rect src_rect = { m_TilePos.col * m_TilePos.w, m_TilePos.row * m_TilePos.h, m_TilePos.w, m_TilePos.h };
            SDL_Rect dst_rect = { static_cast<int>(m_DstRect.x), static_cast<int>(m_DstRect.y), m_DstRect.w, m_DstRect.h };
            Renderer::GetInstance()->Draw(m_TextureID, src_rect, dst_rect, m_Rotation, nullptr,m_Flip);
        };
        void Clean() override {};
        void Update(float dt) override {};

        TilePos& GetTilePos() { return m_TilePos; }
        Rect& GetDstRect() { return m_DstRect; }

        float& GetX() { return m_DstRect.x; }
        float& GetY() { return m_DstRect.y; }
        void SetX(int x) { m_DstRect.x = x; }
        void SetY(int y) { m_DstRect.y = y; }
        float GetMidPointX() const {
            return (m_DstRect.x + static_cast<float>(m_DstRect.w)/2);
        };
        float GetMidPointY() const {
            return (m_DstRect.y + static_cast<float>(m_DstRect.h)/2);
        };
        int& GetWidth() { return m_DstRect.w; }
        int& GetHeight() { return m_DstRect.h; }
        void SetWidth(int width) { m_DstRect.w = width; }
        void SetHeight(int height) { m_DstRect.h = height; }

        float& GetRotation() { return m_Rotation; }
        void SetRotation(float rotation) { m_Rotation = rotation; }

        std::string GetTextureID() { return m_TextureID; }
        std::string GetID() { return m_ObjectID; }
        void SetID(std::string id) { m_ObjectID = std::move(id); }
        SDL_RendererFlip GetFlip() {return m_Flip;}
        void SetFlip(SDL_RendererFlip flip) {m_Flip = flip;}

        void SetCollider(Collider* collider) { m_Collider = collider; }
        Collider* GetCollider(){return m_Collider;}
        Health* GetHealthObj(){return m_Health;}

    protected:
        Transform* m_Transform;
        TilePos m_TilePos;
        Rect m_DstRect;
        float m_Rotation;
        std::string m_TextureID;
        std::string m_ObjectID;
        SDL_RendererFlip m_Flip;
        Collider* m_Collider;
        Health* m_Health;
};

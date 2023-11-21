#pragma once

#include "IObject.h"
#include "Engine/Physics/Transform.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/TileMap.h"
#include "Health.h"
#include "Engine/Animation/Animation.h"

#include "Engine/Physics/CollisionBox.h"
#include <SDL2/SDL.h>
#include <string>
#include <utility>

#include "Engine/utils/utils.h"

enum class ObjectType {
    None = 0,
    Base, Collider,
    Projectile, Player, Enemy,
    Weapon, RangedWeapon, MeleeWeapon
};

// enum ObjectCategory {
//     kNone = 0,
//     kCollider = 1 << 0,
//     kProjectile = 1 << 1,
//     kCharacter = 1 << 2,
//     kWeapon = 1 << 3
// };

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
            m_Animation(nullptr) {

            m_Transform = new Transform(&m_DstRect.x, &m_DstRect.y);
        }

        explicit GameObject(GameObject* rhs);
        virtual ~GameObject() = default;

        virtual void Draw() override;
        virtual void Clean() override {};
        virtual void Update(float dt) override;

        virtual ObjectType GetObjectType() { return ObjectType::Base; }

        TilePos& GetTilePos() { return m_TilePos; }
        Rect& GetDstRect() { return m_DstRect; }

        float& GetX() { return m_DstRect.x; }
        float& GetY() { return m_DstRect.y; }
        void SetX(float x) { m_DstRect.x = x; }
        void SetY(float y) { m_DstRect.y = y; }
        void MoveX(float x) { m_DstRect.x += x; }
        void MoveY(float y) { m_DstRect.y += y; }
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

        Animation* GetAnimation() { return m_Animation; }
        void SetAnimation(Animation* animation) { m_Animation = animation; }

    protected:
        Transform* m_Transform;
        TilePos m_TilePos;
        Rect m_DstRect;
        float m_Rotation;
        std::string m_TextureID;
        std::string m_ObjectID;
        SDL_RendererFlip m_Flip;
        Animation* m_Animation;
};

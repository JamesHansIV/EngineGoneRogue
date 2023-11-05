#pragma once

#include "IObject.h"
#include "Engine/Components/Physics/Transform.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/TileMap.h"

#include <SDL2/SDL.h>
#include <string>
#include "Engine/Components/Physics/Collider.h"

enum class ObjectType {
    None = 0,
    Base, Projectile, Warrior
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
            std::string objectID = "",
            SDL_RendererFlip flip = SDL_FLIP_NONE
        ){
            TilePosition = tilePos;
            DstRect = dstRect;
            Flip = flip;
            TextureID = textureID;
            ObjectID = objectID;
        }
        std::string ObjectID;
        std::string TextureID;
        TilePos TilePosition;
        Rect DstRect;
        SDL_RendererFlip Flip;
};

class GameObject : public IObject {
    public:
        GameObject(Properties& props): m_TextureID(props.TextureID),
            m_TilePos(props.TilePosition), m_DstRect(props.DstRect),
            m_Flip(props.Flip), m_ObjectID(props.ObjectID){

            m_Transform = new Transform(&m_DstRect.x, &m_DstRect.y);
        }
        virtual ~GameObject() {}

        virtual ObjectType GetObjectType() { return ObjectType::Base; }

        virtual void Draw() {
            SDL_Rect srcRect = { m_TilePos.col * m_TilePos.w, m_TilePos.row * m_TilePos.h, m_TilePos.w, m_TilePos.h };
            SDL_Rect dstRect = { (int)m_DstRect.x, (int)m_DstRect.y, m_DstRect.w, m_DstRect.h };
            Renderer::GetInstance()->Draw(m_TextureID, srcRect, dstRect);
        };
        virtual void Clean() {};
        virtual void Update(float dt) {};

        TilePos& GetTilePos() { return m_TilePos; }
        Rect& GetDstRect() { return m_DstRect; }
        
        float& GetX() { return m_DstRect.x; }
        float& GetY() { return m_DstRect.y; }
        void SetX(int x) { m_DstRect.x = x; }
        void SetY(int y) { m_DstRect.y = y; }
        float GetMidPointX() { 
            return (m_DstRect.x + (float)m_DstRect.w/2);
        };
        float GetMidPointY() { 
            return (m_DstRect.y + (float)m_DstRect.h/2);
        };
        int& GetWidth() { return m_DstRect.w; }
        int& GetHeight() { return m_DstRect.h; }
        void SetWidth(int width) { m_DstRect.w = width; }
        void SetHeight(int height) { m_DstRect.h = height; }

        std::string GetTextureID() { return m_TextureID; }
        std::string GetID() { return m_ObjectID; }
        void SetID(std::string id) { m_ObjectID = id; }
        SDL_RendererFlip getFlip() {return m_Flip;}
        void setFlip(SDL_RendererFlip flip) {m_Flip = flip;}


        Collider* getCollider(){return m_Collider;};

    protected:
        Transform* m_Transform;
        TilePos m_TilePos;
        Rect m_DstRect;
        std::string m_TextureID;
        std::string m_ObjectID;
        SDL_RendererFlip m_Flip;
        Collider* m_Collider;
};

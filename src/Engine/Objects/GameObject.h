#pragma once

#include "IObject.h"
#include "Engine/Components/Physics/Transform.h"
#include "Engine/Renderer/Renderer.h"
#include <SDL2/SDL.h>
#include <string>
#include "Engine/Components/Physics/Collider.h"

enum class ObjectType {
    None = 0,
    Base, Projectile, Warrior
};

struct Properties{
    public:
        Properties(std::string textureID, int x, int y, int width, int height, int dst_width, int dst_height, std::string objectID = "", SDL_RendererFlip flip = SDL_FLIP_NONE){
            X = x;
            Y = y;
            Flip = flip;
            Width = width;
            Height = height;
            DstWidth = dst_width;
            DstHeight = dst_height;
            TextureID = textureID;
            ObjectID = objectID;
        }
        std::string ObjectID;
        std::string TextureID;
        int Width, Height;
        int DstWidth, DstHeight;
        float X, Y;
        SDL_RendererFlip Flip;
};

class GameObject : public IObject {
    public:
        GameObject(Properties& props): m_TextureID(props.TextureID),
            m_Width(props.Width), m_Height(props.Height), m_DstWidth(props.DstWidth), m_DstHeight(props.DstHeight),
            m_Flip(props.Flip), m_ObjectID(props.ObjectID){

            m_Transform = new Transform(props.X, props.Y);
        }

        virtual void Draw() {
            SDL_Rect srcRect = { 0, 0, m_Width, m_Height };
            SDL_Rect dstRect = { (int)m_Transform->X, (int)m_Transform->Y, m_DstWidth, m_DstHeight };

            Renderer::GetInstance()->Draw(m_TextureID, srcRect, dstRect);
        };
        virtual void Clean() {};
        virtual void Update(float dt) {};

        float& GetX() { return m_Transform->X; }
        float& GetY() { return m_Transform->Y; }
        void SetX(float x) { m_Transform->X = x; }
        void SetY(float y) { m_Transform->Y = y; }
        float GetMidPointX() { 
            return (m_Transform->X + (float)m_DstWidth/2);
        };
        float GetMidPointY() { 
            return (m_Transform->Y + (float)m_DstHeight/2);
        };
        int& GetWidth() { return m_DstWidth; }
        int& GetHeight() { return m_DstHeight; }
        void SetWidth(int width) { m_DstWidth = width; }
        void SetHeight(int height) { m_DstHeight = height; }

        std::string GetTextureID() { return m_TextureID; }
        std::string GetID() { return m_ObjectID; }
        void SetID(std::string id) { m_ObjectID = id; }
        SDL_RendererFlip getFlip() {return m_Flip;};
        void setFlip(SDL_RendererFlip flip) {m_Flip = flip;};

        Collider* getCollider(){return m_Collider;};

    protected:
        Transform* m_Transform;
        int m_Width, m_Height;
        int m_DstWidth, m_DstHeight;
        std::string m_TextureID;
        std::string m_ObjectID;
        SDL_RendererFlip m_Flip;
        Collider* m_Collider;
};

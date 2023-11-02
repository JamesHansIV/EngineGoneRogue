#pragma once

#include "IObject.h"
#include "Engine/Components/Physics/Transform.h"
#include "Engine/Renderer/Renderer.h"
#include <SDL2/SDL.h>
#include <string>

struct Properties{
    public:
        Properties(std::string textureID, int x, int y, int width, int height, int dst_width, int dst_height, SDL_RendererFlip flip = SDL_FLIP_NONE){
            X = x;
            Y = y;
            Flip = flip;
            Width = width;
            Height = height;
            DstWidth = dst_width;
            DstHeight = dst_height;
            TextureID = textureID;
        }

        std::string TextureID;
        int Width, Height;
        int DstWidth, DstHeight;
        float X, Y;
        SDL_RendererFlip Flip;
};

class GameObject : public IObject {
    public:
        GameObject(Properties& props): m_TextureID(props.TextureID),
            m_Width(props.Width), m_Height(props.Height), m_DstWidth(props.DstWidth), m_DstHeight(props.DstHeight), m_Flip(props.Flip), m_ObjectID(0){

            m_Transform = new Transform(props.X, props.Y);
        }

        virtual void Draw() {
            SDL_Rect srcRect = { 0, 0, m_Width, m_Height };
            SDL_Rect dstRect = { (int)m_Transform->X, (int)m_Transform->Y, m_DstWidth, m_DstHeight };

            Renderer::GetInstance()->Draw(m_TextureID, srcRect, dstRect);
        };
        virtual void Clean() {};
        virtual void Update(float dt) {};

        int GetID() { return m_ObjectID; }
        void SetID(int id) { m_ObjectID = id; }

    protected:
        Transform* m_Transform;
        int m_Width, m_Height;
        int m_DstWidth, m_DstHeight;
        std::string m_TextureID;
        int m_ObjectID;
        SDL_RendererFlip m_Flip;
};

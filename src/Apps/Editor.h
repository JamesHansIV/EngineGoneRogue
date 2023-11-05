#pragma once

#include "Events/Event.h"
#include "Application.h"
#include "Engine/Renderer/Texture.h"
#include <set>

class Editor : public Application{
    public:
        Editor();
        ~Editor();

        virtual void Update(float dt);
        virtual void Render();
        virtual void Events();

        void OnMouseClicked(SDL_Event& event);
        void OnMouseMoved(SDL_Event& event);

        TilePos ChooseTile(TileMap* tileMap);
        void ShowMenuBar();
        void ShowLoadTexture();
        void ShowLoadTilemap();
        ObjectType ShowSelectObjectType();
        void ShowCreateBaseObject(TilePos tilePos, Rect dstRect);
        void ShowCreateProjectile(TilePos tilePos, Rect dstRect);
        void ShowCreatePlayer(TilePos tilePos, Rect dstRect);
        void ShowCreateObject();
        void CreateObject(ObjectType type);
        void ShowObjectEditor();
        void ShowTextureIDs();
        void ShowObjectManager();
    private:
        Map* m_Map;
        std::vector<std::string> m_TextureIDs;
        Texture* m_CurrentTexture;
        GameObject* m_CurrentObject;
        std::vector<GameObject*> m_Objects;
};


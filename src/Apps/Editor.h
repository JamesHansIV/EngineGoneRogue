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


        TilePos ChooseTile(TileMap* tileMap);
        void ShowMenuBar();
        void ShowLoadTexture();
        void ShowLoadTilemap();
        void ShowSelectObjectType();
        void ShowCreateBaseObject();
        void ShowCreateProjectile();
        void ShowCreatePlayer();
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


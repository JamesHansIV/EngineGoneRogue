#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Application/Application.h"
#include "Engine/Renderer/Texture.h"
#include <set>

struct ObjectInfo {
    TilePos Tile;
    Rect DstRect;
    ObjectType type;
};

class Editor : public Application{
    public:
        Editor();
        ~Editor();

        virtual void Update(float dt);
        virtual void Render();
        virtual void Events();

        void OnMouseClicked(SDL_Event& event);
        void OnMouseMoved(SDL_Event& event);

        void SetObjectInfo();

        void ShowMenuBar();
        void ShowChooseLayer();
        void ShowLoadTexture();
        void ShowLoadTilemap();
        ObjectType ShowSelectObjectType();
        void ShowCreateBaseObject();
        void ShowCreateProjectile();
        void ShowCreatePlayer();
        void ShowCreateObject();
        void CreateObject(ObjectType type);
        void ShowObjectEditor();
        void ShowTextureIDs();
        void ShowObjectManager();

        void SaveProject();
        void ShowSaveProject();
    private:
        Map* m_Map;
        std::vector<std::string> m_TextureIDs;
        Texture* m_CurrentTexture;
        GameObject* m_CurrentObject;
        ObjectInfo m_ObjectInfo;
        std::vector< std::vector<GameObject*>> m_Layers;
        std::set<int> m_HiddenLayers;
        int m_CurrentLayer;
        std::vector<GameObject*> m_Objects;
};


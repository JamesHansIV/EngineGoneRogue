#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Application/Application.h"
#include "Engine/Renderer/Texture.h"
#include <set>

struct ObjectInfo {
    TilePos Tile;
    Rect DstRect;
    float Rotation = 0.0f;
    ObjectType type = ObjectType::Base;
    bool SnapToGrid = true;
};

class Editor : public Application{
    public:
        Editor();
        ~Editor();

        void CleanLayers();

        virtual void Update(float dt);
        virtual void Render();
        virtual void Events();

        void OnMouseClicked(SDL_Event& event);
        void OnMouseMoved(SDL_Event& event);
        void OnMouseUp(SDL_Event& event);

        void SetObjectInfo();

        void ShowFileManager();
        void ShowChooseLayer();
        void ShowLoadTexture();
        void ShowLoadTilemap();
        void ShowTiles(TileMap* tilemap);
        ObjectType ShowSelectObjectType();
        void ShowCreateBaseObject();
        void ShowCreateProjectile();
        void ShowCreatePlayer();
        void ShowCreateObject();
        void CreateObject(ObjectType type);
        void ShowObjectEditor();
        void ShowTextureIDs();
        void ShowObjectManager();

        void AddCurrentRoom();
        void SaveRoom(const char* roomName);
        void SaveProject();
    private:
        std::string m_CurrentRoomID;
        Texture* m_CurrentTexture;
        GameObject* m_CurrentObject;
        ObjectInfo m_ObjectInfo;
        std::vector< std::vector<GameObject*>> m_Layers;
        std::set<int> m_HiddenLayers;
        int m_CurrentLayer;
        std::vector<GameObject*> m_Objects;
};


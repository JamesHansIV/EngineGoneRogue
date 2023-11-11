#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Application/Application.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Objects/Projectile.h"
#include "Engine/Objects/Player.h"
#include <set>

struct ObjectInfo {
    TilePos Tile;
    Rect DstRect;
    float Rotation = 0.0f;
    ObjectType type = ObjectType::Base;
    bool SnapToGrid = true;
};

struct DrawState {
    bool DrawMode = false;
    bool IsDrawing = false;
    float PrevX = -1;
    float PrevY = -1;
};

class Editor : public Application{
    public:
        Editor();
        ~Editor();

        void CleanLayers();

        virtual void Update(float dt);
        virtual void Render();
        virtual void Events();

        std::pair<float, float> SnapToGrid(float x, float y);

        void OnMouseClicked(SDL_Event& event);
        void OnMouseMoved(SDL_Event& event);
        void OnMouseUp(SDL_Event& event);

        void SetObjectInfo();
        void AddObject(float x, float y);

        void ShowFileManager();
        void ShowChooseLayer();
        void ShowLoadTexture();
        void ShowLoadTilemap();
        void ShowTiles(TileMap* tilemap);
        ObjectType ShowSelectObjectType();
        void ShowBuildProjectile();
        void ShowBuildPlayer();
        void ShowCreateObject();
        void DeleteObject();
        void CreateObject(ObjectType type);
        // Player* CreatePlayer(Properties props);
        // Projectile* CreateProjectile(Properties props);
        void ShowObjectEditor();
        void ShowTextureIDs();
        void ShowObjectManager();

        void CreateProjectFolder();
        void AddRoom();
        void SaveRoom(const char* roomName);
        void SaveProject();
    private:
        std::string m_CurrentRoomID;
        Texture* m_CurrentTexture;
        GameObject* m_CurrentObject;
        DrawState m_DrawState;
        ObjectInfo m_ObjectInfo;
        std::vector< std::vector<GameObject*>> m_Layers;
        std::set<int> m_HiddenLayers;
        int m_CurrentLayer;
};

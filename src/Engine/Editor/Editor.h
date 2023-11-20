#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Application/Application.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Objects/Projectile.h"
#include "Engine/Objects/Player.h"
#include <set>

struct EnemyInfo {
    int PerceptionWidth;
    int PerceptionHeight;
};

struct AnimationInfo {
    TilePos Tile;
    int FrameCount;
    int AnimationSpeed;
};

struct ObjectInfo {
    TilePos Tile;
    Rect DstRect;
    float Rotation = 0.0F;
    ObjectType type = ObjectType::kBase;
    SDL_Rect CollisionBox;
    AnimationInfo Animation;
    bool SnapToGrid = true;
};

enum class EditMode {
    NONE = 0,
    DRAW,
    ERASE,
    SELECT
};

struct EditState {
    EditMode EditMode = EditMode::NONE;
    bool IsEditing = false;
    float PrevX = -1;
    float PrevY = -1;
};

class Editor : public Application{
    public:
        Editor();
        ~Editor() override;

        void CleanLayers();

        void Update(float dt) override;
        void Render() override;
        void Events() override;

        static std::pair<float, float> SnapToGrid(float x, float y);

        GameObject* GetObjectUnderMouse();

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
        void ShowBuildEnemy();
        static void ShowBuildPlayer();
        void ShowCreateObject();
        void DeleteObject(GameObject* obj);
        void CreateObject(ObjectType type);
        // Player* CreatePlayer(Properties props);
        // Projectile* CreateProjectile(Properties props);
        void ShowAddCollider();
        void ShowAddAnimation();
        void ShowObjectEditor();
        void ShowTextureIDs();
        void ShowObjectManager();

        void CreateProjectFolder();
        void AddRoom();
        void SaveRoom(const char* roomName);
        void SaveProject();
    private:
        std::string m_CurrentRoomID;
        Texture* m_CurrentTexture{nullptr};
        GameObject* m_CurrentObject;
        std::set<GameObject*> m_SelectedObjects;
        EditState m_DrawState;
        ObjectInfo m_ObjectInfo;
        EnemyInfo m_EnemyInfo;
        std::vector< std::vector<GameObject*>> m_Layers;
        std::set<int> m_HiddenLayers;
        int m_CurrentLayer{0};
};


#pragma once

#include <set>
#include "Engine/Application/Application.h"
#include "Engine/Events/Event.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/Projectiles/Projectile.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Cursors/Cursor.h"
#include "Engine/KeyMap/KeyMap.h"
#include "Engine/Cursors/Cursor.h"

#include "Engine/Editor/EditMode.h"

#include <tuple>

struct TileCoords {
    int row;
    int col;


    bool operator==(const TileCoords& rhs) const {
        return (row == rhs.row && col == rhs.col);
    }

    bool operator!=(const TileCoords& rhs) const {
        return !(row == rhs.row && col == rhs.col);
    }
};



struct E_EnemyInfo {
    int PerceptionWidth;
    int PerceptionHeight;
};

struct E_AnimationInfo {
    TilePos Tile;
    int FrameCount;
    int AnimationSpeed;
};

struct E_ObjectInfo {
    TilePos Tile;
    Rect DstRect;
    float Rotation = 0.0F;
    ObjectType type = ObjectType::Base;
    SDL_Rect CollisionBox;
    E_AnimationInfo Animation;
    bool SnapToGrid = true;
};

struct EditState {
    EditMode EditMode = EditMode::NONE;
    bool IsEditing = false;
    float PrevX = -1;
    float PrevY = -1;
};

class Editor : public Application {
   public:
    Editor();
    ~Editor() override;

    void CleanLayers();

    void PrintLayer();
    void PrintLayer(int row, int col);

    void Update(float dt) override;
    void Render() override;
    void Events() override;

    static std::pair<float, float> SnapToGrid(float x, float y);
    static void SnapToGrid(float x, float y, GameObject* obj);
    std::pair<int, int>PixelToTilePos(float x, float y);
    TileCoords PixelToTileCoords(float x, float y);

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
    void DeleteObject(GameObject* obj, int layer_num);
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

    bool LoadEditorTextures();

   private:
    std::string m_CurrentRoomID;
    Texture* m_CurrentTexture{nullptr};
    GameObject* m_CurrentObject;
    std::set<GameObject*> m_SelectedObjects;
    EditState m_EditState;
    E_ObjectInfo m_ObjectInfo;
    E_EnemyInfo m_EnemyInfo;
    std::vector<std::vector<GameObject*>> m_Layers;
    std::set<int> m_HiddenLayers;
    std::unordered_map<std::string, std::pair<int,int>>m_CursorOffsets;
    int m_CurrentLayer{0};
    TileCoords m_MouseInputOrigin;


    void CheckForToolSelection(EditorAction editor_action, EditMode edit_mode);

    // Action Handlers
    void HandleDrawAction();
    void HandleEraseAction();
    void HandleNoToolActions(bool mouse_moved, SDL_Event& event); // handles logic when editmode is none
    void HandleTileSelectAction(bool mouse_moved,  SDL_Event& event);
    void HandleDragMoveAction(SDL_Event& event);

    // action handler helpers
    bool SelectTile(int x, int y); // returns true if selection is made, false if no selection is made, x & y are tile coords
    bool IsMouseOverASelectedTile (TileCoords coords);
    void StopEditing();
    void UpdateEditMode(EditMode mode, bool isEditing);

    std::tuple<float, float>GetMousePixelPos();
    TileCoords GetMouseTilePos();

    // keymap
    KeyMap* m_KeyMap;

    // map
    Cursor* m_Cursor;
};

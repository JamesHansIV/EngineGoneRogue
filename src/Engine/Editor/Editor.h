#pragma once

#include <set>
#include "Engine/Application/Application.h"
#include "Engine/Events/Event.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/Projectiles/Projectile.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Cursors/Cursor.h"
#include "Engine/KeyMap/KeyMap.h"

struct EEnemyInfo {
    int PerceptionWidth;
    int PerceptionHeight;
};

struct EAnimationInfo {
    TilePos Tile;
    int FrameCount;
    int AnimationSpeed;
};

struct EObjectInfo {
    TilePos Tile;
    Rect DstRect;
    float Rotation = 0.0F;
    ObjectType type = ObjectType::Base;
    SDL_Rect CollisionBox;
    EAnimationInfo Animation;
    bool SnapToGrid = true;
};

enum class EditMode { NONE = 0, DRAW, ERASE, SELECT };

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
    static std::pair<int, int>PixelToTilePos(float x, float y);

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

    static bool LoadEditorTextures();

   private:
    std::string m_current_room_id;
    Texture* m_current_texture{nullptr};
    GameObject* m_current_object;
    std::set<GameObject*> m_selected_objects;
    EditState m_edit_state;
    EObjectInfo m_object_info;
    EEnemyInfo m_enemy_info;
    std::vector<std::vector<GameObject*>> m_layers;
    std::set<int> m_hidden_layers;
    int m_current_layer{0};


    void CheckForToolSelection(EditorAction editor_action, EditMode edit_mode);

    // keymap
    KeyMap* m_key_map;

    // map
    Cursor* m_cursor;
};

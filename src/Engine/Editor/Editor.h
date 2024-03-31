#pragma once

#include <set>
#include "Engine/Application/Application.h"
#include "Engine/Events/Event.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/Projectiles/Projectile.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Cursors/Cursor.h"
#include "Engine/KeyMap/KeyMap.h"
#include "Engine/Editor/TileCoords.h"
#include "Engine/Editor/ActionRecordHandling/ActionRecordHandler.h"

#include "Engine/Editor/EditMode.h"
#include "Engine/utils/utils.h"
#include "Engine/ClipBoard/ClipBoard.h"

#include "Engine/Editor/UI/Toolbar.h"

#include <tuple>

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

struct EditState {
    EditMode EditMode = EditMode::NONE;
    bool IsEditing = false;
    float PrevX = -1;
    float PrevY = -1;
    TileCoords PrevCoords {};
};

class Editor : public Application {
   public:
    Editor();
    ~Editor() override;

    void LoadFromLayers();

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
    std::pair<float, float>TileCoordsToPixels(TileCoords coords); // returns {x,y}

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

    void ShowToolBar();
    void ShowRibbon();
    // void ShowObjectEditor(); // NEEDS NEW NAME

    void CreateProjectFolder();
    void AddRoom();
    bool SaveRoom(const char* roomName);
    void SaveProject();
    void NewProject(std::string project_name);
    void LoadProject(std::string project_name);

    bool LoadEditorTextures();

    // static std::vector<std::vector<GameObject*>>GetLayers() { return m_layers; }

   private:
    std::string m_current_room_id;
    Texture* m_current_texture{nullptr};
    GameObject* m_current_object;
    std::set<GameObject*> m_selected_objects;
    std::unordered_map<GameObject*, std::pair<float, float>>m_selected_obj_origin_map; //x,y
    EditState m_edit_state;
    EObjectInfo m_object_info;
    EEnemyInfo m_enemy_info;
    std::vector<std::vector<GameObject*>> m_layers;
    std::set<int> m_hidden_layers;
    int m_current_layer{0};
    std::unordered_map<std::string, std::pair<int,int>>m_cursor_offsets;
    TileCoords m_mouse_input_origin;
    ClipBoard* m_clipboard;
    Toolbar* m_toolbar;

    bool m_success = false;


    


    // static std::vector<std::vector<GameObject*>>* s_layers;

    void CheckForToolSelection(EditorAction editor_action, EditMode edit_mode);

    // Action Handlers
    void HandleDrawAction();
    void HandleEraseAction();
    void HandleNoToolActions(bool mouse_moved, SDL_Event& event); // handles logic when editmode is none
    void HandleTileSelectAction(bool mouse_moved,  SDL_Event& event);
    void HandleDragMoveAction(SDL_Event& event);
    void HandlePaintBucketAction(SDL_Event& event);
    void HandleDeleteSelectionAction();
    void HandleCopySelectionAction();
    void HandleCutSelectionAction();
    void HandlePasteClipboardAction();

    // action handler helpers
    bool SelectTile(int x, int y, bool multi_select_enabled); // returns true if selection is made, false if no selection is made, x & y are tile coords
    bool IsMouseOverASelectedTile (TileCoords coords);
    bool IsTileEmpty(TileCoords coords);
    void StopEditing();
    void UpdateEditMode(EditMode mode, bool isEditing);
    std::vector<GameObject*> GetObjectsOnTile(int row, int col);

    std::tuple<float, float>GetMousePixelPos();
    TileCoords GetMouseTilePos();

    // keymap
    KeyMap* m_key_map;

    // map
    Cursor* m_cursor;

    // undo redo
    ActionRecordHandler* m_action_record_handler;

    std::string m_help_popup_text;
    void BuildHelpPopUpText();

    bool m_show_create_object;
    bool m_show_object_editor = false;
};

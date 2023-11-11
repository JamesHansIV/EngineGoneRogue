#include "Application.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Objects/Player.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Input/InputChecker.h"

#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "Engine/utils/utils.h"

#include <unistd.h>
#include <dirent.h>

Application* Application::m_Instance = nullptr;

Application::Application() : m_ProjectName("test_project") {

    if(SDL_Init(SDL_INIT_VIDEO)!=0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)!= 0){
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        assert(false);
    }

    m_Window = SDL_CreateWindow("Engine Gone Rogue", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(m_Window == nullptr){
        SDL_Log("Failed to create Window: %s", SDL_GetError());
        assert(false);
    }

    m_Instance = this;

    //TODO: note that the cwd is <projectDir>/build instead of <projectDir>.
    //      Set a working directory path macro to use absolute file paths
    Renderer::GetInstance()->Init();

    if (LOAD_PROJECT) {
        if (!LoadProject()) {
            SDL_Log("Failed to load project");
            assert(false);
        }
    }
    
    m_IsRunning = true;
}

bool Application::LoadTextures(char* projectPath) {
    tinyxml2::XMLDocument doc;
    std::string texturesPath = "";
    texturesPath += projectPath;
    texturesPath += "/textures.xml";
    SDL_Log("%s", texturesPath.c_str());
    tinyxml2::XMLError error = doc.LoadFile(texturesPath.c_str());
    if (error != tinyxml2::XML_SUCCESS) {
        SDL_LogError(0, "Could not load textures");
        return false;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("Root");
    tinyxml2::XMLElement* currTexture = root->FirstChildElement("Texture");

    std::string id = "";
    std::string texturePath = "";
    while (currTexture != nullptr) {
        id = currTexture->FirstChildElement("ID")->GetText();
        texturePath = currTexture->FirstChildElement("FilePath")->GetText();
        Renderer::GetInstance()->AddTexture(id, texturePath);
        currTexture = currTexture->NextSiblingElement("Texture");
    }
    return true;
}

bool Application::LoadObject(tinyxml2::XMLElement* xmlObj, std::string roomID) {
    tinyxml2::XMLElement* textureID = xmlObj->FirstChildElement("TextureID");
    tinyxml2::XMLElement* objectID = xmlObj->FirstChildElement("ObjectID");
    tinyxml2::XMLElement* srcRect = xmlObj->FirstChildElement("SrcRect");
    tinyxml2::XMLElement* dstRect = xmlObj->FirstChildElement("DstRect");
    tinyxml2::XMLElement* rotation = xmlObj->FirstChildElement("Rotation");

    std::string textureIDVal = textureID->GetText();
    std::string objectIDVal = objectID->GetText();
    SDL_Log("Texture id: %s", textureIDVal.c_str());
    SDL_Log("Object id: %s", objectIDVal.c_str());
    TilePos tilePos;
    tilePos.row = atoi(srcRect->FirstChildElement("Row")->GetText());
    tilePos.col = atoi(srcRect->FirstChildElement("Column")->GetText());
    tilePos.w = atoi(srcRect->FirstChildElement("Width")->GetText());
    tilePos.h = atoi(srcRect->FirstChildElement("Height")->GetText());

    SDL_Log("Src row: %d", tilePos.row);
    SDL_Log("Src col: %d", tilePos.col);
    SDL_Log("Src w: %d", tilePos.w);
    SDL_Log("Src h: %d", tilePos.h);

    Rect dstRectVal;
    dstRectVal.x = std::stof(dstRect->FirstChildElement("XPos")->GetText());
    dstRectVal.y = std::stof(dstRect->FirstChildElement("YPos")->GetText());
    dstRectVal.w = atoi(dstRect->FirstChildElement("Width")->GetText());
    dstRectVal.h = atoi(dstRect->FirstChildElement("Height")->GetText());

    SDL_Log("Dst x: %f", dstRectVal.x);
    SDL_Log("Dst y: %f", dstRectVal.y);
    SDL_Log("dst w: %d", dstRectVal.w);
    SDL_Log("dst h: %d", dstRectVal.h);

    float angle = std::stof(rotation->GetText());

    Properties props(
        textureIDVal, tilePos,
        dstRectVal, angle, objectIDVal
    );

    m_Rooms[roomID].push_back(new GameObject(props));
    return true;
}

bool Application::LoadObjects(std::string roomPath, std::string roomID) {
    tinyxml2::XMLDocument doc;

    SDL_Log("%s", roomPath.c_str());
    tinyxml2::XMLError error = doc.LoadFile(roomPath.c_str());
    if (error != tinyxml2::XML_SUCCESS) {
        SDL_LogError(0, "Could not load objects");
        return false;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("Root");
    tinyxml2::XMLElement* currObject = root->FirstChildElement("Object");

    while (currObject != nullptr) {
        if (!LoadObject(currObject, roomID)) return false;
        currObject = currObject->NextSiblingElement("Object");
    }
    return true;

}

bool Application::LoadRooms(char* projectPath) {
    struct dirent* entry;
    DIR* dp;

    std::string roomsPath = projectPath;
    roomsPath += "/rooms";
    dp = opendir(roomsPath.c_str());
    if (dp == NULL) {
        perror("Rooms path does not exist");
        return false;
    }
    
    std::string roomPath = roomsPath;
    std::string roomID = "";
    int index = 0;
    while ((entry = readdir(dp))) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            std::string fileName = entry->d_name;
            roomPath += "/";
            roomPath += entry->d_name;
            roomID = fileName.substr(0, fileName.rfind("."));
            m_Rooms[roomID] = std::vector<GameObject*>();
            if (!LoadObjects(roomPath, roomID)) return false;
            roomPath = roomsPath;
        }
    }

    closedir(dp);
    return true;
}

bool Application::LoadProject() {
    char projectPath[128];
    sprintf(projectPath, "../assets/projects/%s", m_ProjectName.c_str());
    if (!LoadTextures(projectPath)) return false;
    SDL_Log("Textures are fine");
    SDL_Log("%s", projectPath);
    if (!LoadRooms(projectPath)) return false;
    return true;
}

void Application::Events(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        m_EventManager.handleEvent(event);
        switch (event.type) {
            case SDL_QUIT:
                Quit();
                return;
            case SDL_KEYDOWN:
                InputChecker::setKeyPressed(event.key.keysym.sym, true);
                break;
            case SDL_KEYUP:
                InputChecker::setKeyPressed(event.key.keysym.sym, false);
                break;
            case SDL_MOUSEBUTTONDOWN:
                InputChecker::setMouseButtonPressed(event.button.button, true);
                break;
            case SDL_MOUSEBUTTONUP:
                InputChecker::setMouseButtonPressed(event.button.button, false);
                break;
            case SDL_MOUSEMOTION:
                InputChecker::updateMousePosition(event.motion.x, event.motion.y);
                break;
        }
    }
}

void Application::Run() {
    while (m_IsRunning) {
        Events();
        Update(0.5);
        Render();
    }
    Clean();
}

bool Application::Clean(){
    Renderer::GetInstance()->Destroy();

    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();
    return true;
}

void Application::Quit(){
    m_IsRunning = false;
}


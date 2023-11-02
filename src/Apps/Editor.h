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

        void ShowTilemap();
    private:
        std::string m_TextureID;
        Map* m_Map;
        std::set<std::string> m_TextureIDs;
        Texture* m_CurrentTexture;
        std::vector<std::unique_ptr<GameObject>> m_Objects;
};


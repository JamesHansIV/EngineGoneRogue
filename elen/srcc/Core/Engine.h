#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 500

class Engine{
    public:
        static Engine* GetInstance() 
        {
            return m_Instance = (m_Instance != nullptr) ? m_Instance : new Engine();
        }

        bool Init();
        bool Clean();
        void Quit();

        void Update(float dt);
        void Render();
        void Events();

        inline bool IsRunning(){return m_IsRunning;};
        inline SDL_Renderer* GetRenderer(){return m_Renderer;};

    
    private:
        Engine(){};
        bool m_IsRunning;

        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;
        static Engine* m_Instance;
};
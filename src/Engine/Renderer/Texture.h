#pragma once

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <string>

class Texture
{
	public:
		Texture(const std::string& path, std::string id);
		Texture(const char* path, std::string id);
		~Texture();

		inline std::string GetID() const { return m_ID; }
		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
		inline int GetObjectCount() const { return m_ObjectCount; }
		void IncObjectCount() { m_ObjectCount++; }
		virtual SDL_Texture* GetTexture() { return m_Texture; }

	private:
		std::string m_ID;
		SDL_Texture* m_Texture;
		int m_ObjectCount;
		int m_Width;
		int m_Height;
};


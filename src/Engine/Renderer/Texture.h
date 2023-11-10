#pragma once

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <string>

class Texture
{
	public:
		Texture(const std::string& path, std::string id);
		Texture(const char* path, std::string id);
		virtual ~Texture();

		std::string GetID() const { return m_ID; }
		std::string GetFilePath() const { return m_FilePath; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		int GetObjectCount() const { return m_ObjectCount; }
		void IncObjectCount() { m_ObjectCount++; }
		SDL_Texture* GetTexture() { return m_Texture; }

	private:
		std::string m_ID;
		std::string m_FilePath;
		SDL_Texture* m_Texture;
		int m_ObjectCount;
		int m_Width;
		int m_Height;
};


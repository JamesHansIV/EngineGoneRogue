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
		void SetTileWidth(int tileWidth) { m_TileWidth = tileWidth; }
		void SetTileHeight(int tileHeight) { m_TileHeight = tileHeight; }
		void IncObjectCount() { m_ObjectCount++; }
		SDL_Texture* GetTexture() { return m_Texture; }

	private:
		std::string m_ID;
		SDL_Texture* m_Texture;
		int m_ObjectCount;
		int m_Width;
		int m_Height;
		int m_TileWidth;
		int m_TileHeight;
};


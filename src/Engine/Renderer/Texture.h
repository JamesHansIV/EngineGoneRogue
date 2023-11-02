#pragma once

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <string>

class Texture
{
	public:
		Texture(const std::string& path);
		Texture(const char* path);
		~Texture();

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
		void SetTileWidth(int tileWidth) { m_TileWidth = tileWidth; }
		void SetTileHeight(int tileHeight) { m_TileHeight = tileHeight; }
		SDL_Texture* GetTexture() { return m_Texture; }

	private:
		SDL_Texture* m_Texture;
		int m_Width;
		int m_Height;
		int m_TileWidth;
		int m_TileHeight;
};


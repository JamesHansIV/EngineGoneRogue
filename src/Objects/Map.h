#include <string>

struct Coord {
    int x;
    int y;
};

class Map {
    public:
        Map(std::string textureID) : m_TextureID(textureID), m_Width(0), m_Height(0), m_Grid(nullptr) { }
        ~Map();

        bool LoadMap(std::string filename);
        void Draw();

    private:
        std::string m_TextureID;
        int m_Width;
        int m_Height;
        Coord** m_Grid;
};
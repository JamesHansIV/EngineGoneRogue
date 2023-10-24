#include <unordered_map>
#include <vector>
#include "LTexture.h"

class TextureHandler {
    public:
    TextureHandler();
    ~TextureHandler();
    void loadTextures(std::vector<std::string> filepaths);
    
    private:
    std::unordered_map<std::string, LTexture> textures;
};
#pragma once

#include <tinyxml2.h>

#include <dirent.h>
#include <sys/stat.h>
#include <map>
#include <string>

class Collider;
class GameObject;
class Texture;

void WriteColliderInfo(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* xmlObj,
                       Collider* obj);

void WriteBaseObjectInfo(tinyxml2::XMLDocument& doc,
                         tinyxml2::XMLElement* xmlObj, GameObject* obj);

int SaveObjects(const char* filepath, std::vector<GameObject*> tiles);

int SaveTextureMap(const char* filepath,
                   std::map<std::string, Texture*> textureMap);

std::vector<GameObject*> LoadObjects(const char* filepath);

bool LoadTextures(const char* filepath);

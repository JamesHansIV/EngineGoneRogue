#pragma once

#include <tinyxml2.h>

#include <dirent.h>
#include <sys/stat.h>
#include <map>
#include <string>

#include "Engine/Objects/Characters/EnemyStats.h"

class Collider;
class GameObject;
class Texture;

void WriteColliderInfo(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* xmlObj,
                       Collider* obj);

void WriteBaseObjectInfo(tinyxml2::XMLDocument& doc,
                         tinyxml2::XMLElement* xmlObj, GameObject* obj);

void WriteStatsInfo(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* xmlObj,
                        EnemyStats stats);

void WriteStatsInfo(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* xmlObj,
                        RangedEnemyStats stats);

int SaveObjects(const char* filepath, const std::vector<GameObject*>& objects);

int SaveTextureMap(const char* filepath,
                   const std::map<std::string, Texture*>& textureMap);

std::vector<GameObject*> LoadObjects(const char* filepath);

bool LoadTextures(const char* projectPath);

std::string LoadStartRoom(const char* path, int& x, int& y);

std::pair<int, int> LoadStartPosition(const char* path);

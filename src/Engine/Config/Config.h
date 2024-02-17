#pragma once

#include <tinyxml2.h>

#include <dirent.h>
#include <sys/stat.h>
#include <string>

class Collider;
class GameObject;

void WriteColliderInfo(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* xmlObj,
                       Collider* obj);

void WriteBaseObjectInfo(tinyxml2::XMLDocument& doc,
                         tinyxml2::XMLElement* xmlObj, GameObject* obj);

bool SaveObjects(const char* filepath, std::vector<GameObject*> tiles);

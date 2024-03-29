#pragma once
#include <unordered_map>
#include <vector>
class Player;

class GrenadeCounter {
   public:
    GrenadeCounter() = default;

    void Draw();

    void Clean();

    void Update(const Player& player);

   private:
    int m_grenade_count;
};

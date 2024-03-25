#pragma once

#include "Engine/Objects/Characters/Player.h"

class ExperienceBar {
   public:
    explicit ExperienceBar(int max_xp, int xp = 0) : m_max_xp(max_xp), m_xp(xp) {}

    void SetExperience(int xp) { m_xp = xp; }

    void SetMaxExperience(int xp) { m_max_xp = xp; }

    void Draw() const;

    void Update(const Player& player);

   private:
    int m_max_xp;
    int m_xp;
    int m_level;
};

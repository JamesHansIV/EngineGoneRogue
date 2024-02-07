#pragma once

class ExperienceBar {
   public:
    ExperienceBar(int max_xp, int xp = 0) : m_max_xp(max_xp), m_xp(xp) {}

    void SetExperience(int xp) { m_xp = xp; }

    void SetMaxExperience(int xp) { m_max_xp = xp; }

    void Draw() const;

   private:
    int m_max_xp;
    int m_xp;
};

#pragma once

#include "Character.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Physics/RigidBody.h"
#include "functional"
#include "Weapon.h"

class Player: public Character{

    public:
        explicit Player(Properties& props);
        virtual ~Player() override;
        virtual void Draw() override;
        virtual void Clean() override;
        virtual void Update(float dt) override;

        void UpdateWeapon(float dt);

        void CheckInput();

        void OnKeyPressed(SDL_Event& event);
        void OnKeyReleased(SDL_Event& event);
        virtual void OnCollide(Collider* collidee) override;

        bool IsMarkedForDeletion(){return m_MarkedForDeletion;}
        virtual ObjectType GetObjectType() override { return ObjectType::kPlayer; }
    private:
        std::vector<Weapon*> m_Weapons;
        Weapon* m_CurrentWeapon;
        bool m_MarkedForDeletion;
        // TODO: add this to player state when game state is added.
        int m_able_to_dash = 0;
        bool m_is_dashing = false;
        int m_multiplier = 1;
};


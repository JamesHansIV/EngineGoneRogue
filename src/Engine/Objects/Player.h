#pragma once

#include "Character.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Physics/RigidBody.h"
#include "functional"
#include "Weapon.h"

class Player: public Character, public EventListener{

    public:
        explicit Player(Properties& props);
        void Draw() override;
        void Clean() override;
        void Update(float dt) override;
        void UpdateColliders(const std::vector<Collider*>& colliders){m_Colliders = colliders;}
        void OnEvent(Event& event) override;
        bool IsMarkedForDeletion(){return m_MarkedForDeletion;}
        virtual ObjectType GetObjectType() override { return ObjectType::kBase; }
    private:
        void CanMoveThrough();
        void DrawPlayerHealth();
        void DrawWeapon();
        std::vector<Collider*> m_Colliders;
        WeaponType m_CurrentWeapon;
        bool m_MarkedForDeletion;
        // TODO: add this to player state when game state is added.
        int m_able_to_dash = 0;
        bool m_is_dashing = false;
};


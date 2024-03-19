#include "Item.h"
#include "Engine/Objects/Characters/Player.h"

const std::unordered_map<ItemType, std::string> Item::kItemTypeToTexture = {
    {ItemType::PiercingShard, "piercing"},
    {ItemType::RegenElixir, "hp-regen"},
    {ItemType::LifestealStone, "life-steal"},
    {ItemType::RangedBoost, "ranged-boost"},
    {ItemType::MeleeBoost, "melee-boost"},
    {ItemType::ArmorEssence, "armor"},
    {ItemType::SwiftBoots, "swift-boots"},
    {ItemType::DodgeCharm, "dodge-charm"},
};

const std::unordered_map<ItemType, std::string> Item::kItemTypeToDescription = {
    {ItemType::PiercingShard, "Increase your piercing ability by 1"},
    {ItemType::RegenElixir, "Increase your HP regen rate by 1"},
    {ItemType::LifestealStone, "Increase your Life Steal percentage by 5"},
    {ItemType::RangedBoost, "Increase your Ranged Damage by 5"},
    {ItemType::MeleeBoost, "Increase your Melee Damage by 5"},
    {ItemType::ArmorEssence, "Increase your Armor Percentage by 10"},
    {ItemType::SwiftBoots, "Increase your Speed by 20"},
    {ItemType::DodgeCharm, "Increase your Dodge Speed by 1"},
};

const std::unordered_map<ItemType, std::string> Item::kItemTypeToName = {
    {ItemType::PiercingShard, "Piercing Shard"},
    {ItemType::RegenElixir, "Regen Elixir"},
    {ItemType::LifestealStone, "Lifesteal Stone"},
    {ItemType::RangedBoost, "Ranged Boost"},
    {ItemType::MeleeBoost, "Melee Boost"},
    {ItemType::ArmorEssence, "Armor Essence"},
    {ItemType::SwiftBoots, "Swift Boots"},
    {ItemType::DodgeCharm, "Dodge Charm"},
};

void Item::Apply(Player* player) {
    switch (m_item_type) {
        case ItemType::PiercingShard: {
            int const piercing = player->GetMutableStats().GetPiercing();
            player->GetMutableStats().SetPiercing(piercing + 1);
            int const new_piercing = player->GetMutableStats().GetPiercing();
            SDL_Log("Increased piercing from %d to %d", piercing, new_piercing);
            break;
        }
        case ItemType::RegenElixir: {
            int const hp_regen = player->GetMutableStats().GetHPRegenRate();
            player->GetMutableStats().SetHpRegenRate(hp_regen + 1);
            int const new_hp_regen = player->GetMutableStats().GetHPRegenRate();
            SDL_Log("Increased HP regeneration rate from %d to %d", hp_regen, new_hp_regen);
            break;
        }
        case ItemType::LifestealStone: {
            int const life_steal = player->GetMutableStats().GetLifeStealPercentage();
            player->GetMutableStats().SetLifeStealPercentage(life_steal + 5);
            int const new_life_steal = player->GetMutableStats().GetLifeStealPercentage();
            SDL_Log("Increased life steal percentage from %d to %d", life_steal, new_life_steal);
            break;
        }
        case ItemType::RangedBoost: {
            int const ranged_damage = player->GetMutableStats().GetRangedDamage();
            player->GetMutableStats().SetRangedDamage(ranged_damage + 5);
            int const new_ranged_damage = player->GetMutableStats().GetRangedDamage();
            SDL_Log("Increased ranged damage from %d to %d", ranged_damage, new_ranged_damage);
            break;
        }
        case ItemType::MeleeBoost: {
            int const melee_damage = player->GetMutableStats().GetMeleeDamage();
            player->GetMutableStats().SetMeleeDamage(melee_damage + 5);
            int const new_melee_damage = player->GetMutableStats().GetMeleeDamage();
            SDL_Log("Increased melee damage from %d to %d", melee_damage, new_melee_damage);
            break;
        }
        case ItemType::ArmorEssence: {
            int const armor_percentage = player->GetMutableStats().GetArmorPercentage();
            player->GetMutableStats().SetArmorPercentage(armor_percentage + 10);
            int const new_armor_percentage = player->GetMutableStats().GetArmorPercentage();
            SDL_Log("Increased armor percentage from %d to %d", armor_percentage, new_armor_percentage);
            break;
        }
        case ItemType::SwiftBoots: {
            float const speed = player->GetMutableStats().GetSpeed();
            player->GetMutableStats().SetSpeed(speed + 20);
            float const new_speed = player->GetMutableStats().GetSpeed();
            SDL_Log("Increased speed from %.2f to %.2f", speed, new_speed);
            break;
        }
        case ItemType::DodgeCharm: {
            float const dodge_speed = player->GetMutableStats().GetDodgeSpeed();
            player->GetMutableStats().SetDodgeSpeed(dodge_speed + 1);
            float const new_dodge_speed = player->GetMutableStats().GetDodgeSpeed();
            SDL_Log("Increased dodge speed from %.2f to %.2f", dodge_speed, new_dodge_speed);
            break;
        }
        default:
            break;
    }
}
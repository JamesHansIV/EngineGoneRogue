#include "Item.h"
#include "Engine/Objects/Characters/Player.h"

const std::unordered_map<ItemType, std::string> Item::kItemTypeToTexture = {
    {ItemType::Item1, "sniper"},
    {ItemType::Item2, "wooden-chest-idle"},
    {ItemType::Item3, "healthpotion"},
    {ItemType::Item4, "healthpotion"},
    {ItemType::Item5, "wooden-chest-idle"},
    {ItemType::Item6, "sniper"},
    {ItemType::Item7, "sniper"},
    {ItemType::Item8, "sniper"},
    {ItemType::Item9, "wooden-chest-idle"},
    {ItemType::Item10, "wooden-chest-idle"},
    {ItemType::Item11, "healthpotion"},
    {ItemType::Item12, "healthpotion"}
};

const std::unordered_map<ItemType, std::string> Item::kItemTypeToDescription = {
    {ItemType::Item1, "This item increases your piercing ability by 1"},
    {ItemType::Item2, "This item increases your HP regen rate by 1 "},
    {ItemType::Item3, "This item increases your Life Steal percentage by 5"},
    {ItemType::Item4, "This item increases your Ranged Damage by 5"},
    {ItemType::Item5, "This item increases your Melee Damage by 5"},
    {ItemType::Item6, "This item increases your Armor Percentage by 10"},
    {ItemType::Item7, "This item increases your Speed by 20"},
    {ItemType::Item8, "This item increases your Dodge Speed by 1"},
    {ItemType::Item9, "Description of Item 9"},
    {ItemType::Item10, "Description of Item 10"},
    {ItemType::Item11, "Description of Item 11"},
    {ItemType::Item12, "Description of Item 12"}
};

const std::unordered_map<ItemType, std::string> Item::kItemTypeToName = {
    {ItemType::Item1, "Increase Piercing"},
    {ItemType::Item2, "Increase HP Regen"},
    {ItemType::Item3, "Increase Life Steal"},
    {ItemType::Item4, "Increase Ranged Damage"},
    {ItemType::Item5, "Increase Melee Damage"},
    {ItemType::Item6, "Increase Armor Percentage"},
    {ItemType::Item7, "Increase Speed"},
    {ItemType::Item8, "Increase Dodge Speed"},
    {ItemType::Item9, "Item 9"},
    {ItemType::Item10, "Item 10"},
    {ItemType::Item11, "Item 11"},
    {ItemType::Item12, "Item 12"}
};

void Item::Apply(Player* player) {
    switch (m_item_type) {
        case ItemType::Item1: {
            int const piercing = player->GetMutableStats().GetPiercing();
            player->GetMutableStats().SetPiercing(piercing + 1);
            int const new_piercing = player->GetMutableStats().GetPiercing();
            SDL_Log("Increased piercing from %d to %d", piercing, new_piercing);
            break;
        }
        case ItemType::Item2: {
            int const hp_regen = player->GetMutableStats().GetHPRegenRate();
            player->GetMutableStats().SetHpRegenRate(hp_regen + 1);
            int const new_hp_regen = player->GetMutableStats().GetHPRegenRate();
            SDL_Log("Increased HP regeneration rate from %d to %d", hp_regen, new_hp_regen);
            break;
        }
        case ItemType::Item3: {
            int const life_steal = player->GetMutableStats().GetLifeStealPercentage();
            player->GetMutableStats().SetLifeStealPercentage(life_steal + 5);
            int const new_life_steal = player->GetMutableStats().GetLifeStealPercentage();
            SDL_Log("Increased life steal percentage from %d to %d", life_steal, new_life_steal);
            break;
        }
        case ItemType::Item4: {
            int const ranged_damage = player->GetMutableStats().GetRangedDamage();
            player->GetMutableStats().SetRangedDamage(ranged_damage + 5);
            int const new_ranged_damage = player->GetMutableStats().GetRangedDamage();
            SDL_Log("Increased ranged damage from %d to %d", ranged_damage, new_ranged_damage);
            break;
        }
        case ItemType::Item5: {
            int const melee_damage = player->GetMutableStats().GetMeleeDamage();
            player->GetMutableStats().SetMeleeDamage(melee_damage + 5);
            int const new_melee_damage = player->GetMutableStats().GetMeleeDamage();
            SDL_Log("Increased melee damage from %d to %d", melee_damage, new_melee_damage);
            break;
        }
        case ItemType::Item6: {
            int const armor_percentage = player->GetMutableStats().GetArmorPercentage();
            player->GetMutableStats().SetArmorPercentage(armor_percentage + 10);
            int const new_armor_percentage = player->GetMutableStats().GetArmorPercentage();
            SDL_Log("Increased armor percentage from %d to %d", armor_percentage, new_armor_percentage);
            break;
        }
        case ItemType::Item7: {
            float const speed = player->GetMutableStats().GetSpeed();
            player->GetMutableStats().SetSpeed(speed + 20);
            float const new_speed = player->GetMutableStats().GetSpeed();
            SDL_Log("Increased speed from %.2f to %.2f", speed, new_speed);
            break;
        }
        case ItemType::Item8: {
            float const dodge_speed = player->GetMutableStats().GetDodgeSpeed();
            player->GetMutableStats().SetDodgeSpeed(dodge_speed + 1);
            float const new_dodge_speed = player->GetMutableStats().GetDodgeSpeed();
            SDL_Log("Increased dodge speed from %.2f to %.2f", dodge_speed, new_dodge_speed);
            break;
        }
        case ItemType::Item9:
        case ItemType::Item10:
        case ItemType::Item11:
        case ItemType::Item12:
        default:
            break;
    }
}
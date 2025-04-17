#include "../include/Item.h"
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>

std::string Item::getRarityName(ItemRarity rarity) {
    switch (rarity) {
        case ItemRarity::COMMON: return "Common";
        case ItemRarity::RARE: return "Rare";
        case ItemRarity::LEGENDARY: return "Legendary";
        default: return "Unknown";
    }
}

std::string Item::getWeaponTypeName(WeaponType type) {
    switch (type) {
        case WeaponType::SWORD: return "Sword";
        case WeaponType::AXE: return "Axe";
        case WeaponType::MACE: return "Mace";
        case WeaponType::DAGGER: return "Dagger";
        case WeaponType::SPEAR: return "Spear";
        default: return "Unknown";
    }
}

std::string Item::getArmorTypeName(ArmorType type) {
    switch (type) {
        case ArmorType::HELMET: return "Helmet";
        case ArmorType::CHESTPLATE: return "Chestplate";
        case ArmorType::GAUNTLETS: return "Gauntlets";
        case ArmorType::GREAVES: return "Greaves";
        case ArmorType::SHIELD: return "Shield";
        default: return "Unknown";
    }
}

std::string Item::getStatusEffectName(StatusEffect effect) {
    switch (effect) {
        case StatusEffect::BLEEDING: return "Bleeding";
        case StatusEffect::BLINDNESS: return "Blindness";
        case StatusEffect::POISON: return "Poison";
        case StatusEffect::NONE: return "";
        default: return "";
    }
}

void Item::generateStatsForRarity(ItemRarity rarity, int& diceCount, int& diceSides, int& modifier) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    switch (rarity) {
        case ItemRarity::COMMON: {
            std::uniform_int_distribution<> diceDis(1, 2);
            std::uniform_int_distribution<> modDis(-3, 0);
            diceCount = diceDis(gen);
            diceSides = 6;
            modifier = modDis(gen);
            break;
        }
        case ItemRarity::RARE: {
            std::uniform_int_distribution<> diceDis(2, 4);
            std::uniform_int_distribution<> modDis(-1, 2);
            diceCount = diceDis(gen);
            diceSides = 6;
            modifier = modDis(gen);
            break;
        }
        case ItemRarity::LEGENDARY: {
            std::uniform_int_distribution<> diceDis(4, 7);
            std::uniform_int_distribution<> modDis(2, 5);
            diceCount = diceDis(gen);
            diceSides = 6;
            modifier = modDis(gen);
            break;
        }
    }
}

Item::Item(ItemType type, ItemRarity rarity) 
    : type(type), rarity(rarity), diceCount(1), diceSides(6), modifier(0), value(0),
      weaponType(WeaponType::SWORD), armorType(ArmorType::CHESTPLATE) {
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    if (type == ItemType::WEAPON) {
        // Randomly select a weapon type
        std::uniform_int_distribution<> weaponDis(0, 4);
        weaponType = static_cast<WeaponType>(weaponDis(gen));
        
        name = getRarityName(rarity) + " " + getWeaponTypeName(weaponType);
        std::uniform_int_distribution<> statusChanceDis(1, 100);
        if (statusChanceDis(gen) <= 35) { // 35% chance to have a status effect
            std::uniform_int_distribution<> effectDis(1, 3); // skip 0 = NONE
            statusEffect = static_cast<StatusEffect>(effectDis(gen));
            
            // Prepend effect tag to weapon name
            name = "[" + getStatusEffectName(statusEffect) + "] " + name;
        }
        // Generate random stats based on rarity
        generateStatsForRarity(rarity, diceCount, diceSides, modifier);
        
        // Calculate value based on stats
        value = (diceCount * diceSides + modifier) * 10;
    } else { // ARMOR
        // Randomly select an armor type
        std::uniform_int_distribution<> armorDis(0, 4);
        armorType = static_cast<ArmorType>(armorDis(gen));
        name = getRarityName(rarity) + " " + getArmorTypeName(armorType);
        
        // Generate random stats based on rarity
        generateStatsForRarity(rarity, diceCount, diceSides, modifier);
        
        // Calculate value based on stats
        value = (diceCount * diceSides + modifier) * 8;
    }
}

Item::Item(ItemType type, const std::string& itemName, int fixedDiceCount, int fixedDiceSides, int fixedModifier)
    : type(type), rarity(ItemRarity::COMMON), name(itemName),
      diceCount(fixedDiceCount), diceSides(fixedDiceSides), modifier(fixedModifier),
      value((fixedDiceCount * fixedDiceSides + fixedModifier) * (type == ItemType::WEAPON ? 10 : 8)),
      weaponType(WeaponType::SWORD), armorType(ArmorType::CHESTPLATE) {
}

std::string Item::getName() const { return name; }
ItemType Item::getType() const { return type; }
ItemRarity Item::getRarity() const { return rarity; }
int Item::getValue() const { return value; }
int Item::getDiceCount() const { return diceCount; }
int Item::getDiceSides() const { return diceSides; }
int Item::getModifier() const { return modifier; }
StatusEffect Item::getStatusEffect() const { return statusEffect; }
void Item::addDice() {
    diceCount++;
    std::cout << "Added a dice! New stats: " << getStatsString() << "\n";
}

void Item::addModifier(int amount) {
    modifier += amount;
    std::cout << "Added " << amount << " to modifier! New stats: " << getStatsString() << "\n";
}

int Item::roll() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, diceSides);
    
    int total = 0;
    for (int i = 0; i < diceCount; ++i) {
        total += dis(gen);
    }
    return total + modifier;
}

std::string Item::getStatsString() const {
    return "(" + std::to_string(diceCount) + "d" + 
           std::to_string(diceSides) + 
           (modifier >= 0 ? "+" : "") + 
           std::to_string(modifier) + ")";
}

Item Item::generateRandomItem() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    // Randomly choose between weapon and armor
    std::uniform_int_distribution<> typeDis(0, 1);
    ItemType type = static_cast<ItemType>(typeDis(gen));
    
    // Randomly choose rarity
    std::uniform_int_distribution<> rarityDis(0, 2);
    ItemRarity rarity = static_cast<ItemRarity>(rarityDis(gen));
    
    return Item(type, rarity);
} 
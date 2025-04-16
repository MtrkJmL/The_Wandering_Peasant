#pragma once
#include <string>
#include <random>
#include <ctime>

enum class ItemType {
    WEAPON,
    ARMOR
};

enum class ItemRarity {
    COMMON,
    RARE,
    LEGENDARY
};
enum class StatusEffect {
    NONE,
    BLEEDING,
    BLINDNESS,
    POISON
};
// Weapon types
enum class WeaponType {
    SWORD,
    AXE,
    MACE,
    DAGGER,
    SPEAR
};

// Armor types
enum class ArmorType {
    HELMET,
    CHESTPLATE,
    GAUNTLETS,
    GREAVES,
    SHIELD
};

class Item {
private:
    std::string name;
    ItemType type;
    ItemRarity rarity;
    int diceCount;
    int diceSides;
    int modifier;
    int value;

    // For weapons
    WeaponType weaponType;
    
    // For armor
    ArmorType armorType;

    // Add NONE to your enum if not present
    StatusEffect statusEffect = StatusEffect::NONE;

    static std::string getRarityName(ItemRarity rarity);
    static int getDiceCountForRarity(ItemRarity rarity);
    static int getModifierForRarity(ItemRarity rarity);
    static std::string getWeaponTypeName(WeaponType type);
    static std::string getArmorTypeName(ArmorType type);
    static void generateStatsForRarity(ItemRarity rarity, int& diceCount, int& diceSides, int& modifier);

public:
    Item(ItemType type, ItemRarity rarity);
    Item(ItemType type, const std::string& itemName, int fixedDiceCount, int fixedDiceSides, int fixedModifier);
    ~Item() = default;
    
    std::string getName() const;
    ItemType getType() const;
    ItemRarity getRarity() const;
    int getValue() const;
    int getDiceCount() const;
    int getDiceSides() const;
    int getModifier() const;
    StatusEffect getStatusEffect() const;
    static std::string getStatusEffectName(StatusEffect effect);
    void addDice();
    void addModifier(int amount);
    
    int roll() const;
    std::string getStatsString() const;
    
    static Item generateRandomItem();
}; 
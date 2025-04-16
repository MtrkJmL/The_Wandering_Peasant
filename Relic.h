#pragma once
#include <string>

enum class RelicRarity {
    COMMON,
    RARE,
    LEGENDARY
};

class Relic {
private:
    std::string name;
    std::string description;
    RelicRarity rarity;
    bool isActive;
    
public:
    Relic();  // Default constructor
    Relic(const std::string& name, const std::string& description, RelicRarity rarity);
    
    std::string getName() const;
    std::string getDescription() const;
    RelicRarity getRarity() const;
    bool isActiveRelic() const;
    
    // Relic effects
    int modifyStamina(int currentStamina, int maxStamina) const;
    bool shouldDoubleLightAttack() const;
    bool shouldTripleFirstAttack() const;
    bool shouldReroll() const;
    int modifyGoldLoot(int goldAmount) const;
    int modifyDefense(int defense) const;
    bool shouldRepeatAttack(int attackCount) const;
    int modifyDamage(int damage, int currentHealth, int maxHealth) const;
    bool shouldMissAttack() const;
    bool shouldStackBleeding() const;
    float modifyHeavyAttackMultiplier() const;
    
    static Relic generateCommonRelic();
    static Relic generateRareRelic();
    static Relic generateLegendaryRelic();
}; 
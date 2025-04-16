#include "../include/Relic.h"
#include <random>
#include <ctime>
#include <iostream>

Relic::Relic() : name(""), description(""), rarity(RelicRarity::COMMON), isActive(false) {}

Relic::Relic(const std::string& name, const std::string& description, RelicRarity rarity)
    : name(name), description(description), rarity(rarity), isActive(false) {
}

std::string Relic::getName() const {
    return name;
}

std::string Relic::getDescription() const {
    return description;
}

RelicRarity Relic::getRarity() const {
    return rarity;
}

bool Relic::isActiveRelic() const {
    return isActive;
}

// Relic effects
int Relic::modifyStamina(int currentStamina, int maxStamina) const {
    if (name == "[🔵]Stamina Crystal") {
        
    }
    return currentStamina;
}

bool Relic::shouldDoubleLightAttack() const {
    if (name == "[⚪]Sharp Edge") {
        return (std::rand() % 100) < 10;
    }
    return false;
}

bool Relic::shouldTripleFirstAttack() const {
    return name == "[⚪]Warrior's Spirit";
}

bool Relic::shouldReroll() const {
    return name == "[⚪]Lucky Coin";
}

int Relic::modifyGoldLoot(int goldAmount) const {
    if (name == "[⚪]Lucky Coin") {
        return goldAmount * 2;
    }
    return goldAmount;
}

int Relic::modifyDefense(int defense) const {
    if (name == "[⚪]Iron Heart") {
        return defense + 1;
    }
    return defense;
}

bool Relic::shouldRepeatAttack(int attackCount) const {
    if (name == "[🔵]Echo Strike") {
        return (std::rand() % 100) < 35;
    }
    return false;
}

int Relic::modifyDamage(int damage, int currentHealth, int maxHealth) const {
    if (name == "[🔵]Blood Chalice" && currentHealth < maxHealth / 2) {
        std::cout << "You rolled a " << damage << " and Blood Chalice is active.\n";
        return damage * 1.5f;
    }
    if (name == "[🟠]Dragon's Heart") {
        std::cout << "You rolled a " << damage << " and Dragon's Heart is active.\n";
        return damage * 2;
    }
    if (name == "[⚪]Sharp Edge") {
        std::cout << "You rolled a " << damage << " and Sharp Edge added 1 to it.\n";
        return damage + 1;
    }
    if (name == "[🟠]Crown of Command" && currentHealth < maxHealth / 2) {
        std::cout << "You rolled a " << damage << " and Crown of Command is active.\n";
        return damage * 3;
    }
    return damage;
}

bool Relic::shouldMissAttack() const {
    return false;
}

bool Relic::shouldStackBleeding() const {
    return name == "[🔵]Reaper's Scythe";
}

float Relic::modifyHeavyAttackMultiplier() const {
    if (name == "[🟠]Crown of Command") {
        std::cout << "Crown of Command is active.\n";
        return 3.0f;
    }
    return 1.0f;
}

Relic Relic::generateCommonRelic() {
    // Example common relics
    static const std::vector<Relic> commonRelics = {
        Relic("[⚪]Lucky Coin", "Gold rewards are doubled", RelicRarity::COMMON),
        Relic("[⚪]Iron Heart", "Gain +2 defense", RelicRarity::COMMON),
        Relic("[⚪]Sharp Edge", "+1 damage to all attacks, 10% chance to double light attacks", RelicRarity::COMMON),
        Relic("[⚪]Warrior's Spirit", "First attack in battle deals double damage", RelicRarity::COMMON),
        Relic("[⚪]Traveler's Boots", "+1 stamina", RelicRarity::COMMON),
        Relic("[⚪]Worn Bandage", "Resting also restores 10 health during combat", RelicRarity::COMMON)
    };
    
    return commonRelics[std::rand() % commonRelics.size()];
}

Relic Relic::generateRareRelic() {
    // Example rare relics
    static const std::vector<Relic> rareRelics = {
        Relic("[🔵]Blood Chalice", "Attacks deal x1.5 damage when under 50% health", RelicRarity::RARE),
        Relic("[🔵]Echo Strike", "35% chance to repeat an attack", RelicRarity::RARE),
        Relic("[🔵]Stamina Crystal", "Resting increases stamina by 1", RelicRarity::RARE),
        Relic("[🔵]Reaper's Scythe", "Bleeding attacks can stack", RelicRarity::RARE),
        Relic("[🔵]Traveler's Mighty Boots", "+3 stamina", RelicRarity::RARE)
    };
    
    return rareRelics[std::rand() % rareRelics.size()];
}

Relic Relic::generateLegendaryRelic() {
    // Example legendary relics
    static const std::vector<Relic> legendaryRelics = {
        Relic("[🟠]Dragon's Heart", "All attacks deal double damage", RelicRarity::LEGENDARY),
        Relic("[🟠]KIRIK's Blessing", "Almighty KIRIK blesses your weapon", RelicRarity::LEGENDARY),
        Relic("[🟠]Stadin Relic", "does nothing but looks cool", RelicRarity::LEGENDARY),
        Relic("[🟠]Crown of Command", "Attacks deal triple damage under 50% health", RelicRarity::LEGENDARY)
    };
    
    return legendaryRelics[std::rand() % legendaryRelics.size()];
} 
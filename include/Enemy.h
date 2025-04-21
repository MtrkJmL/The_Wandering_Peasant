#pragma once
#include <string>
#include "Item.h"

enum class EnemyType {
    GOBLIN,
    ORC,
    TROLL,
    BANDIT,
    SKELETON,
    DRAGON,
    SIR_G_P_SQUIRE
};

class Enemy {
private:
    std::string name;
    int level;
    int health;
    int maxHealth;
    int goldReward;
    int experienceReward;
    bool bleedingStatus;
    bool blindStatus = false;
    int poisonStacks;
    int poisonDamage;
    Item weapon;
    Item armor;
    EnemyType enemyType;
    ItemRarity dropRarity;
    bool canBeTalkedTo;
    int bribeCost;

public:
    Enemy();
    Enemy(const std::string& name, int level, EnemyType type = EnemyType::GOBLIN);
    
    std::string getName() const;
    int getLevel() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getGoldReward() const;
    int getExperienceReward() const;
    EnemyType getType() const;
    ItemRarity getDropRarity() const;
    
    void setHealth(int health);
    void takeDamage(int damage);
    bool isAlive() const;
    
    int rollAttack() const;
    int rollDefense() const;
    
    Item generateDrop(int luck) const;

    void applyBleeding();
    bool isBleeding() const;
    void takeBleedingDamage();

    bool isBlinded() const;
    void applyBlindness();
    void blind();

    void applyPoison();
    bool isPoisoned() const;
    void takePoisonDamage();

    void enableTalking();
    bool canTalk() const;
    std::string getDialogue() const;
    int getBribeCost() const;
    bool acceptBribe(int gold);
}; 
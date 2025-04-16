#include "../include/Enemy.h"
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>

Enemy::Enemy() : name("Goblin"), level(1), health(50), maxHealth(50), 
                 goldReward(10), experienceReward(20),
                 enemyType(EnemyType::GOBLIN),
                 dropRarity(ItemRarity::COMMON),
                 weapon(ItemType::WEAPON, "Rusty Dagger", 1, 4, 0),
                 armor(ItemType::ARMOR, "Tattered Cloth", 1, 4, 0) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

Enemy::Enemy(const std::string& name, int level, EnemyType type) 
    : name(name), level(level), health(50 + (level * 25)), maxHealth(50 + (level * 25)),
      goldReward(10 + (level * 5)), experienceReward(20 + (level * 10)),
      bleedingStatus(false),poisonStacks(0), poisonDamage(0),blindStatus(false), enemyType(type), canBeTalkedTo(false),
      bribeCost(level * 100),
      weapon(ItemType::WEAPON, "Rusty Dagger", 1, 6, 0),
      armor(ItemType::ARMOR, "Tattered Cloth", 1, 6, 0) {
    
    // Determine drop rarity based on enemy type and level
    int rarityRoll = std::rand() % 100;
    if (rarityRoll < 50) {
        dropRarity = ItemRarity::COMMON;
    } else if (rarityRoll < 85) {
        dropRarity = ItemRarity::RARE;
    } else {
        dropRarity = ItemRarity::LEGENDARY;
    }
    
    // Scale enemy equipment based on level
    int baseDice = 1 + (level - 1) / 3;
    int baseModifier = level - 1;
    
    // Generate appropriate weapon and armor based on enemy type
    switch (type) {
        case EnemyType::GOBLIN:
            weapon = Item(ItemType::WEAPON, "Rusty Dagger", baseDice, 4, baseModifier);
            armor = Item(ItemType::ARMOR, "Tattered Cloth", baseDice, 4, baseModifier);
            break;
        case EnemyType::ORC:
            weapon = Item(ItemType::WEAPON, "Crude Axe", baseDice + 1, 6, baseModifier + 1);
            armor = Item(ItemType::ARMOR, "Leather Armor", baseDice, 6, baseModifier);
            goldReward += 15;
            experienceReward += 15;
            break;
        case EnemyType::TROLL:
            weapon = Item(ItemType::WEAPON, "Stone Club", baseDice + 2, 8, baseModifier + 2);
            armor = Item(ItemType::ARMOR, "Thick Hide", baseDice + 1, 6, baseModifier + 1);
            goldReward += 30;
            experienceReward += 30;
            break;
        case EnemyType::BANDIT:
            weapon = Item(ItemType::WEAPON, "Stolen Sword", baseDice + 1, 8, baseModifier + 1);
            armor = Item(ItemType::ARMOR, "Leather Armor", baseDice, 6, baseModifier);
            goldReward += 20;
            experienceReward += 20;
            break;
        case EnemyType::SKELETON:
            weapon = Item(ItemType::WEAPON, "Bone Sword", baseDice, 6, baseModifier+2);
            armor = Item(ItemType::ARMOR, "Bone Armor", baseDice+2, 8, baseModifier+1);
            goldReward += 20;
            experienceReward += 20;
            break;
        case EnemyType::DRAGON:
            weapon = Item(ItemType::WEAPON, "Dragon Claws", baseDice + 3, 10, baseModifier + 3);
            armor = Item(ItemType::ARMOR, "Dragon Scales", baseDice + 2, 8, baseModifier + 2);
            goldReward += 50;
            experienceReward += 50;
            break;
    }
}

std::string Enemy::getName() const { return name; }
int Enemy::getLevel() const { return level; }
int Enemy::getHealth() const { return health; }
int Enemy::getMaxHealth() const { return maxHealth; }
int Enemy::getGoldReward() const { return goldReward; }
int Enemy::getExperienceReward() const { return experienceReward; }
EnemyType Enemy::getType() const { return enemyType; }
ItemRarity Enemy::getDropRarity() const { return dropRarity; }

void Enemy::setHealth(int newHealth) {
    health = std::min(maxHealth, std::max(0, newHealth));
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

bool Enemy::isAlive() const {
    return health > 0;
}

int Enemy::rollAttack() const {
    return weapon.roll();
}

int Enemy::rollDefense() const {
    return armor.roll();
}

Item Enemy::generateDrop() const {
    
    int roll = std::rand() % 100;
    ItemRarity rarity;
    int mod = 0;
    switch (enemyType) {
        case EnemyType::DRAGON:
            mod = 30;            
            break;
        case EnemyType::BANDIT:
            mod = 15;
            break;
        case EnemyType::SKELETON:
            mod = 15;
            break;
        case EnemyType::TROLL:
            mod = 20;
            break;
        case EnemyType::ORC:
            mod = 10;
            break;
        default:
            break;
    }   
    if (roll < 60-mod) {
        rarity = ItemRarity::COMMON;
    } else if (roll < 85) {
        rarity = ItemRarity::RARE;
    } else {
        rarity = ItemRarity::LEGENDARY;
    }
    
    // Randomly choose between weapon and armor
    ItemType type = (std::rand() % 2 == 0) ? ItemType::WEAPON : ItemType::ARMOR;
    return Item(type, rarity);
}

void Enemy::applyBleeding() {
    bleedingStatus = true;
}

bool Enemy::isBleeding() const {
    return bleedingStatus;
}

void Enemy::takeBleedingDamage() {
    if (bleedingStatus) {
        health -= 2;
        if (health < 0) health = 0;
    }
}

bool Enemy::isBlinded() const {
    return blindStatus;
}

void Enemy::applyBlindness() {
    blindStatus = true;
}

void Enemy::blind() {
    blindStatus = false;
}

bool Enemy::isPoisoned() const {
    return poisonStacks > 0;
}

void Enemy::applyPoison() {
    poisonStacks += 5;
}

void Enemy::takePoisonDamage() {
    health -= poisonStacks; 
    std::cout << "Toxic pain wracks the enemy’s body, their movements sluggish and unsure. The enemy takes " << poisonStacks << " damage from poison!\n";
    poisonStacks -= 1;
    if (health < 0) health = 0;
}

void Enemy::enableTalking() {
    canBeTalkedTo = true;
}

bool Enemy::canTalk() const {
    return canBeTalkedTo;
}

std::string Enemy::getDialogue() const {
    switch (enemyType) {
        case EnemyType::GOBLIN:
            return "Goblin: 'Me no want fight! Me just hungry! You give food?'";
        case EnemyType::ORC:
            return "Orc: 'You strong! Maybe we no fight? We share treasure?'";
        case EnemyType::TROLL:
            return "Troll: 'Troll no like fight. Troll like shiny things. You give shiny?'";
        case EnemyType::BANDIT:
            return "Bandit: 'Look, we don't have to do this. I'm just trying to make a living.'";
        case EnemyType::SKELETON:
            return "Skeleton: '...' (It seems to be trying to communicate, but you can't understand it)";
        case EnemyType::DRAGON:
            return "Dragon: 'Foolish mortal! Do you dare to speak to me?'";
        default:
            return "The enemy seems confused by your attempt to communicate.";
    }
}

int Enemy::getBribeCost() const {
    return bribeCost;
}

bool Enemy::acceptBribe(int gold) {
    return gold >= bribeCost;
} 
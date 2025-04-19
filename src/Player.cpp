#include "../include/Player.h"
#include <iostream>
#include <algorithm>

Player::Player(const std::string& playerName)
    : name(playerName), level(1), experience(0), gold(50), luck(0),
      health(100), maxHealth(100), stamina(5), maxStamina(5),
      weapon(ItemType::WEAPON, "Rusty Sword", 1, 6, 2),
      armor(ItemType::ARMOR, "Leather Armor", 1, 6, 0),
      experienceToNextLevel(100), briberySkill(1.0f),
      equippedRelic(), equippedBlessing(), attackCount(0), isFirstAttack(true) {
}

std::string Player::getName() const { return name; }
int Player::getLevel() const { return level; }
int Player::getExperience() const { return experience; }
int Player::getGold() const { return gold; }
int Player::getHealth() const { return health; }
int Player::getMaxHealth() const { return maxHealth; }
int Player::getStamina() const { return stamina; }
int Player::getMaxStamina() const { return maxStamina; }
const Item& Player::getWeapon() const {
    return weapon;
}

Item& Player::getWeapon() {
    return weapon;
}

const Item& Player::getArmor() const {
    return armor;
}

Item& Player::getArmor() {
    return armor;
}

const std::vector<Item>& Player::getInventory() const { return inventory; }
int Player::getExperienceToNextLevel() const { return experienceToNextLevel; }

bool Player::hasUsedBlessing() const {
    return blessingUsed;
}

void Player::useBlessing() {
    blessingUsed = true;
}

void Player::resetBlessingUsed() {
    blessingUsed = false;
}

void Player::takeDamage(int damage) {
    health = std::max(0, health - damage);
}

void Player::heal(int amount) {
    health = std::min(maxHealth, health + amount);
}

void Player::rest() {
    if(equippedRelic.getName() == "[⚪]Worn Bandage") {
        health += 10;
        if(health > maxHealth) {
            health = maxHealth;
        }
        std::cout << "Worn Bandage activates!\n";
    } 
    if(equippedRelic.getName() == "[🔵]Stamina Crystal") {
        maxStamina += 1;
        std::cout << "Stamina Crystal activates!\n";
    }
    stamina = maxStamina; 
    
    std::cout << "You rest and recover your stamina!\n";
    
}

void Player::useStamina(int amount) {
    stamina = std::max(0, stamina - amount);
}

int Player::lightAttack() {
    if (stamina < 1) {
        std::cout << "Not enough stamina for a light attack!\n";
        return 0;
    }
    
    useStamina(1);
    attackCount++;
    
    int damage = weapon.roll() + level;
    
    // Apply relic effects
    if (equippedRelic.shouldDoubleLightAttack()) {
        damage *= 2;
        std::cout << "Blade Charm activates! Attack hits twice!\n";
    }
    
    if (equippedRelic.shouldRepeatAttack(attackCount)) {
        damage *= 2;
        std::cout << "Mask of Echoes activates! Attack is repeated!\n";
    }
    
    if (equippedRelic.modifyDamage(damage, health, maxHealth) != damage) {
        damage = equippedRelic.modifyDamage(damage, health, maxHealth);
        std::cout << "Blood Candle activates! Extra damage!\n";
    }
    
    return damage;
}

int Player::heavyAttack() {
    if (stamina < 2) {
        std::cout << "Not enough stamina for a heavy attack!\n";
        return 0;
    }
    
    useStamina(2);
    attackCount++;
    
    float multiplier = equippedRelic.modifyHeavyAttackMultiplier();
    int damage = static_cast<int>((weapon.roll() * multiplier) + level);
    
    if (equippedRelic.shouldRepeatAttack(attackCount)) {
        damage *= 2;
        std::cout << "Mask of Echoes activates! Attack is repeated!\n";
    }
    
    if (equippedRelic.modifyDamage(damage, health, maxHealth) != damage) {
        damage = equippedRelic.modifyDamage(damage, health, maxHealth);
        std::cout << "Blood Candle activates! Extra damage!\n";
    }
    
    return damage;
}

int Player::rollDefense() const {
    int defense = armor.roll() + level;
    return equippedRelic.modifyDefense(defense);
}

void Player::addExperience(int amount) {
    experience += amount;
    while (experience >= experienceToNextLevel) {
        levelUp();
    }
}
void Player::setMaxStamina(int amount) {
    maxStamina += amount;
    stamina = maxStamina;
}

void Player::setMaxHealth(int amount) {
    maxHealth += amount;
    health = maxHealth;
}

int Player::getLuck() const {
    return luck;
}

void Player::setLuck(int amount) {
    luck += amount;
}

void Player::setBardBeaten(bool beaten) {
    bardBeaten = beaten;
}

bool Player::getBardBeaten() const {
    return bardBeaten;
}

void Player::setMoltoTalked(bool talked) {
    moltoTalked = talked;
}

bool Player::getMoltoTalked() const {
    return moltoTalked;
}

void Player::levelUp() {
    level++;
    experience -= experienceToNextLevel;
    experienceToNextLevel = static_cast<int>(experienceToNextLevel * 1.5);
    
    maxHealth += 20;
    health = maxHealth;
    maxStamina += 1;
    stamina = maxStamina;
    
    std::cout << "\n\n⬆️ Level Up! You are now level " << level << "!\n";
    std::cout << "💙 Health increased to " << maxHealth << "!\n";
    std::cout << "⚡️ Stamina increased to " << maxStamina << "!\n\n";
}

void Player::equipWeapon(const Item& newWeapon) {
    if (newWeapon.getType() == ItemType::WEAPON) {
        weapon = newWeapon;
        std::cout << "Equipped " << newWeapon.getName() << "!\n";
    }
}

void Player::equipArmor(const Item& newArmor) {
    if (newArmor.getType() == ItemType::ARMOR) {
        armor = newArmor;
        std::cout << "Equipped " << newArmor.getName() << "!\n";
    }
}

void Player::addItem(const Item& item) {
    inventory.push_back(item);
}

void Player::removeItem(int index) {
    if (index >= 0 && index < inventory.size()) {
        inventory.erase(inventory.begin() + index);
    }
}

void Player::addGold(int amount) {
    gold += amount;
    if (equippedRelic.getName() == "[⚪]Lucky Coin") {
        gold += amount;
        std::cout << "Lucky Coin activates! You earned extra " << amount << " gold!\n";
    }
}

void Player::pray() {
    maxHealth -= 20;
    health = maxHealth;
    std::cout << "You pray and sacrifice 20 max health.\n";
}

bool Player::spendGold(int amount) {
    if (gold >= amount) {
        gold -= amount;
        return true;
    }
    return false;
}

bool Player::isAlive() const {
    return health > 0;
}

void Player::improveBriberySkill() {
    briberySkill = 0.1f + briberySkill;
}

void Player::learnEnemyType(EnemyType type) {
    if (std::find(knownEnemies.begin(), knownEnemies.end(), type) == knownEnemies.end()) {
        knownEnemies.push_back(type);
    }
}

void Player::learnBardSong(BardSong song) {
    if (std::find(knownBardSongs.begin(), knownBardSongs.end(), song) == knownBardSongs.end()) {
        knownBardSongs.push_back(song);
    }
}

bool Player::knowsEnemyType(EnemyType type) const {
    return std::find(knownEnemies.begin(), knownEnemies.end(), type) != knownEnemies.end();
}

bool Player::knowsBardSong(BardSong song) const {
    return std::find(knownBardSongs.begin(), knownBardSongs.end(), song) != knownBardSongs.end();
}

float Player::getBriberySkill() const {
    return briberySkill;
}

const Relic& Player::getEquippedRelic() const {
    return equippedRelic;
}

const Blessing& Player::getEquippedBlessing() const {
    return equippedBlessing;
}

void Player::equipBlessing(const Blessing& newBlessing) {
    equippedBlessing = newBlessing;
    std::cout << "Equipped " << newBlessing.getName() << ": " << newBlessing.getDescription() << "\n";
}

void Player::equipRelic(const Relic& newRelic) {
    if (newRelic.getName() == "[🔵]Traveler's Mighty Boots") {
        maxStamina += 3;
        stamina = maxStamina;
    }
    else if (newRelic.getName() == "[⚪]Traveler's Boots") {
        maxStamina += 1;
        stamina = maxStamina;
    }
    if(equippedRelic.getName() == "[⚪]Traveler's Boots") {
        maxStamina -= 1;
        stamina = maxStamina;
    }
    else if (equippedRelic.getName() == "[🔵]Traveler's Mighty Boots") {
        maxStamina -= 3;
        stamina = maxStamina;
    }
    equippedRelic = newRelic;
    std::cout << "Equipped " << newRelic.getName() << ": " << newRelic.getDescription() << "\n";
}

void Player::startBattle() {
    attackCount = 0;
    isFirstAttack = true;
}

bool Player::shouldReroll() const {
    return equippedRelic.shouldReroll();
}

int Player::modifyGoldLoot(int goldAmount) const {
    return equippedRelic.modifyGoldLoot(goldAmount);
}

bool Player::shouldMissAttack() const {
    return equippedRelic.shouldMissAttack();
}

bool Player::shouldStackBleeding() const {
    return equippedRelic.shouldStackBleeding();
}

float Player::getTimingMultiplier(int cursorPosition, int totalPositions) const {
    // Middle position is crit (x2)
    // Positions get progressively worse as they move away from center
    float center = totalPositions / 2.0f;
    float distanceFromCenter = std::abs(cursorPosition - center);
    float maxDistance = center;
    
    // Scale from 1.0 (worst) to 2.0 (best)
    return 1.0f + (1.0f - 1.25f*(distanceFromCenter / maxDistance));
}

int Player::timingBasedAttack(int cursorPosition, int totalPositions, bool isHeavyAttack, bool& isCrit) {
    isCrit = false;
    if (isHeavyAttack && stamina < 2) {
        std::cout << "Not enough stamina for a heavy attack!\n";
        return 0;
    } else if (!isHeavyAttack && stamina < 1) {
        std::cout << "Not enough stamina for a light attack!\n";
        return 0;
    }
    
    useStamina(isHeavyAttack ? 2 : 1);
    attackCount++;
    
    float timingMultiplier = getTimingMultiplier(cursorPosition, totalPositions);
    std::cout << "Timing multiplier: " << timingMultiplier << "\n";
    int baseDamage = weapon.roll() + level;
    std::cout << "You rolled a " << baseDamage << ".\n";

    if (isHeavyAttack) {
        baseDamage = static_cast<int>(baseDamage * 1.5f);
        std::cout << "Heavy attack damage: " << baseDamage << "\n";
    }
    
    // Apply relic effects
    int modifiedDamage = equippedRelic.modifyDamage(baseDamage, health, maxHealth);
    if (modifiedDamage != baseDamage) {
        baseDamage = modifiedDamage;
    }
    if (equippedRelic.shouldDoubleLightAttack() && !isHeavyAttack) {
        baseDamage *= 2;
        std::cout << "Sharp Edge activates! Attack hits twice!\n";
    }
    if (equippedRelic.shouldTripleFirstAttack() && attackCount == 1) {
        baseDamage *= 2;
        std::cout << "Warrior's Spirit activates! Attack deals double damage!\n";
    }
    if (equippedRelic.shouldRepeatAttack(attackCount)) {
        baseDamage *= 2;
        std::cout << "Echo Strike activates! Attack is repeated!\n";
    }
    
    // Apply timing multiplier
    int finalDamage = static_cast<int>(baseDamage * timingMultiplier);
    
    if (timingMultiplier >= 1.9f) {
        isCrit = true;
        std::cout << "Perfect timing! Critical hit!\n";
    } else if (timingMultiplier <= 1.1f) {
        std::cout << "Poor timing! Weak hit!\n";
    }
    
    return finalDamage;
} 
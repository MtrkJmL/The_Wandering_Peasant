#pragma once
#include <string>
#include "Item.h"
#include <vector>
#include "Enemy.h"
#include "Relic.h"

class Player {
private:
    std::string name;
    int level;
    int experience;
    int gold;
    int health;
    int maxHealth;
    int stamina;
    int maxStamina;
    int luck;
    Item weapon;
    Item armor;
    std::vector<Item> inventory;
    int experienceToNextLevel;
    float briberySkill; //  represents 0% to 100% discount
    std::vector<EnemyType> knownEnemies; // Types of enemies we can talk to
    Relic equippedRelic;
    int attackCount; // For tracking attack count for relics
    bool isFirstAttack; // For Battle Horn relic

    void levelUp();

public:
    Player(const std::string& playerName);
    
    // Basic stats
    std::string getName() const;
    int getLevel() const;
    int getExperience() const;
    int getGold() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getStamina() const;
    int getMaxStamina() const;
    const Item& getWeapon() const;
    const Item& getArmor() const;
    Item& getWeapon();
    Item& getArmor();
    const std::vector<Item>& getInventory() const;
    int getExperienceToNextLevel() const;
    float getBriberySkill() const;
    bool knowsEnemyType(EnemyType type) const;
    const Relic& getEquippedRelic() const;
    int getLuck() const;
    void setLuck(int amount);
    void setMaxStamina(int amount);
    // Combat actions
    void takeDamage(int damage);
    void heal(int amount);
    void rest(); // Regain stamina
    void useStamina(int amount);
    int lightAttack(); // Cost 1 stamina
    int heavyAttack(); // Cost 2 stamina
    int rollDefense() const;
    
    // Experience and leveling
    void addExperience(int amount);
    void gainExperience(int amount) { addExperience(amount); }
    
    // Equipment
    void equipWeapon(const Item& newWeapon);
    void equipArmor(const Item& newArmor);
    void setWeapon(const Item& newWeapon) { equipWeapon(newWeapon); }
    void setArmor(const Item& newArmor) { equipArmor(newArmor); }
    void equipRelic(const Relic& newRelic);
    
    // Inventory
    void addItem(const Item& item);
    void removeItem(int index);
    void addGold(int amount);
    bool spendGold(int amount);
    
    // Relic effects
    void startBattle();
    bool shouldReroll() const;
    int modifyGoldLoot(int goldAmount) const;
    bool shouldMissAttack() const;
    bool shouldStackBleeding() const;
    
    bool isAlive() const;
    
    // Skill methods
    void improveBriberySkill();
    void learnEnemyType(EnemyType type);
    
    // Timing-based attack methods
    float getTimingMultiplier(int cursorPosition, int totalPositions) const;
    int timingBasedAttack(int cursorPosition, int totalPositions, bool isHeavyAttack, bool& isCrit);
}; 
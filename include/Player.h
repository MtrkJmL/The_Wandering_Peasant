#pragma once
#include <string>
#include "Item.h"
#include <vector>
#include "Enemy.h"
#include "NPC.h"
#include "Relic.h"
#include "Blessing.h"
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
    int defense;
    int luck;
    Item weapon;
    Item armor;
    std::vector<Item> inventory;
    int experienceToNextLevel;
    float briberySkill; //  represents 0% to 100% discount
    std::vector<EnemyType> knownEnemies; // Types of enemies we can talk to
    std::vector<BardSong> knownBardSongs;
    Relic equippedRelic;
    Blessing equippedBlessing;
    bool blessingUsed = false;
    int attackCount; // For tracking attack count for relics
    bool isFirstAttack; // For Battle Horn relic
    bool bardBeaten = false;
    bool moltoTalked = false;

    

public:
    Player(const std::string& playerName);
    void levelUp();
    // Basic stats
    std::string getName() const;
    int getLevel() const;
    int getExperience() const;
    int getGold() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getStamina() const;
    int getMaxStamina() const;
    int getDefense() const;
    const Item& getWeapon() const;
    const Item& getArmor() const;
    Item& getWeapon();
    Item& getArmor();
    const std::vector<Item>& getInventory() const;
    int getExperienceToNextLevel() const;
    float getBriberySkill() const;
    bool knowsEnemyType(EnemyType type) const;
    bool knowsBardSong(BardSong song) const;
    const Relic& getEquippedRelic() const;
    const Blessing& getEquippedBlessing() const;
    int getLuck() const;
    void setLuck(int amount);
    void setMaxStamina(int amount);
    void setMaxHealth(int amount);
    bool hasUsedBlessing() const;
    void useBlessing();
    void resetBlessingUsed();
    // Combat actions
    void takeDamage(int damage);
    void heal(int amount);
    void rest(); // Regain stamina
    void pray();
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
    void equipBlessing(const Blessing& newBlessing);
    
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
    void learnBardSong(BardSong song);
    void setBardBeaten(bool beaten);
    bool getBardBeaten() const;
    void setMoltoTalked(bool talked);
    bool getMoltoTalked() const;
    // Timing-based attack methods
    float getTimingMultiplier(int cursorPosition, int totalPositions) const;
    int timingBasedAttack(int cursorPosition, int totalPositions, bool isHeavyAttack, bool& isCrit);
}; 
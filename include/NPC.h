#pragma once
#include <string>
#include <vector>
#include "Item.h"
#include "Enemy.h"

enum class NPCType {
    BLACKSMITH,
    MERCHANT,
    AHMED
};

class NPC {
private:
    std::string name;
    NPCType type;
    std::string greeting;
    std::string option1;
    std::string option2;
    std::string response1;
    std::string response2;
    
public:
    NPC(NPCType npcType);
    std::string getName() const { return name; }
    NPCType getType() const { return type; }
    std::string getGreeting() const { return greeting; }
    std::string getOption1() const { return option1; }
    std::string getOption2() const { return option2; }
    std::string getResponse1() const { return response1; }
    std::string getResponse2() const { return response2; }
    
    // Interaction methods
    void talk(const std::vector<std::string>& recentEncounters, int totalEnemiesDefeated);
    void upgradeWeapon(Item& weapon, int& playerGold);
    void upgradeArmor(Item& armor, int& playerGold);
    
    // Dialogue options
    void addDialogueOption(const std::string& option);
    std::vector<std::string> getDialogueOptions() const;
    
    // Rewards
    int getGoldCost() const;
    int getExperienceReward() const;
    
    // Ahmed's special methods
    void teachAboutEnemy(EnemyType type);
    void teachBribery();
}; 
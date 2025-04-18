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

enum class BardSong {
    BALLAD_OF_KIRIK,              // The legendary slayer's tale
    WHISPERS_OF_THE_WYRM,         // A haunting tune of old dragons
    ECLIPSEFIRE_LAMENT,           // A sorrowful melody about the Dragon King's wrath
    MARCH_OF_THE_LOST_CROWNS,     // A rousing tale of fallen kings
    DIRGE_OF_THE_DARKBLOODS,      // Mournful song of cursed warriors
    HYMNS_FROM_THE_HOLLOW,        // Echoes from the abyss
    TAVERN_TUNE_OF_GLORY,         // A cheerful classic sung in every inn
    ODE_TO_THE_LAST_FLAME,        // Tribute to the last ember of hope
    DANCE_OF_THE_STARBOUND,       // Mystical rhythm about celestial wanderers
    CHANT_OF_THE_MOONWATCHERS     // Night-born prophecy keepers of Zmar
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
    std::string getBardSongVerse(BardSong song) const;
    std::string getBardSongName(BardSong song) const;
    
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
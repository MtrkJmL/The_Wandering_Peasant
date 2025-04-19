#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "NPC.h"
#include "Relic.h"
#include "Terrain.h"
#include "Village.h"
#include "Quest.h"
#include "QuestManager.h"
#include <vector>
#include <string>
#include <random>
#include <iostream>

class Game {
private:
    Player player;
    std::vector<std::string> recentEncounters;
    int totalEnemiesDefeated;
    bool isRunning;
    int encounterCount;  // Track total encounters
    bool isMiniBossEncounter;  // Flag for mini-boss encounters
    bool isFinalBossEncounter;  // Flag for final boss encounter
    
    void handleWandering();
    void handleBranchingPath();
    void handleEncounter(Enemy& enemy, Terrain& terrain);
    void handleNPCInteraction(NPCType type);
    void handleMarket();
    void handleFinalBoss();
    void gameOver();
    void victory();
    void showStatus();
    void showInventory();
    void saveGame();
    void loadGame();
    void displayMainMenu();
    void startNewGame();
    void displayPlayerStats();
    void rest();
    void handleMiniBoss();
    
public:
    static QuestManager questManager;
    Game();
    void run();
}; 
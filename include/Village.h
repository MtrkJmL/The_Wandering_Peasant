#pragma once   
#include <string>
#include <vector>
#include "NPC.h"
#include "Item.h"
#include "Enemy.h"
#include "Player.h"
#include "Game.h"
#include <limits>

enum class VillagerType {
    BLACKSMITH,
    MERCHANT,
    TAVERN_KEEPER,
    TAVERN_PATRON,
    SHRINE_KEEPER
};

class Player;
class Game;

class Village {
    private:
    std::string name;
    std::vector<NPC> villagers;
    bool hasPrayed = false;
    void displayMenu(Player& player);
    void visitBlacksmith(Player& player);
    void visitShrine(Player& player);
    void visitMarket(Player& player);
    void visitTavern(Player& player);
    void talkToNPCs(Player& player);
    void talkToBartender(Player& player);
    void challengeBard(Player& player);

    public: 
    Village(const std::string& villageName);

    void enter(Player& player, Game& game);
};
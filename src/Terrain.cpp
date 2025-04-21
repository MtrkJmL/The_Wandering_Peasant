#include "../include/Terrain.h"
#include <random>
#include <ctime>
#include <iostream>

Terrain::Terrain(TerrainType type) : type(type), gen(std::random_device{}()) {
    switch (type) {
        case TerrainType::PLAINS:
            name = "Plains";
            description = "A vast open field with no special effects.";
            break;
        case TerrainType::LAVA_FIELD:
            name = "Scorched Crater";
            description = "The intense heat drains your health (-2 HP per turn).";
            break;
        case TerrainType::ICY_VALLEY:
            name = "Stillwhite Valley";
            description = "The slippery terrain makes attacks harder to land (20% chance to miss).";
            break;
        case TerrainType::DARK_FOREST:
            name = "Dark Forest";
            description = "The oppressive atmosphere drains your stamina (-1 stamina per turn).";
            break;
        case TerrainType::SAND_DUNES:
            name = "Sand Dunes";
            description = "The warm desert winds restore your stamina (+1 stamina per turn).";
            break;
        case TerrainType::MYSTIC:
            name = "Echoreach Glade";
            description = "Every movement echoes with past and future lives.";
            break;
        case TerrainType::MOUNTAIN:
            name = "Summit of the Slain";
            description = "The high ground gives you a defensive advantage (+2 defense).";
            break;
        case TerrainType::RAINY_PLAINS:
            name = "Whispering Thicket";
            description = "The gentle rain has healing properties (15% chance to heal).";
            break;
        case TerrainType::SKY_BASTION:
            name = "Ashen Sky Bastion";
            description = "Where Dragon King reborned from the ashes!";
            break;
    }
}

TerrainType Terrain::getType() const {
    return type;
}

std::string Terrain::getName() const {
    return name;
}

std::string Terrain::getDescription() const {
    return description;
}

int Terrain::getHealthModifier() const {
    switch (type) {
        case TerrainType::LAVA_FIELD:
            return -2;
        default:
            return 0;
    }
}

int Terrain::getStaminaModifier() const {
    switch (type) {
        case TerrainType::DARK_FOREST:
            std::cout << "\nThe air grows heavier as the souls of fallen Darkbloods linger, draining your strength" << std::endl;
            return -1;
        case TerrainType::SAND_DUNES:
            std::cout << "\nThe desert's challenges ignite a fire within you, and your body finds new strength in the heat" << std::endl;
            return 1;
        default:
            return 0;
    }
}

int Terrain::getDefenseModifier() const {
    switch (type) {
        case TerrainType::MOUNTAIN:
            return 2;
        default:
            return 0;
    }
}
bool Terrain::shouldEcho() const{
    if (type == TerrainType::MYSTIC) {
        std::uniform_int_distribution<> dis(1, 100);
        return dis(gen) <= 50; // 50% chance to echo
    }
    return false;
}
bool Terrain::shouldMissAttack() const {
    if (type == TerrainType::ICY_VALLEY) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);
        return dis(gen) <= 20; // 20% chance to miss
    }
    return false;
}

bool Terrain::shouldHeal() const {
    if (type == TerrainType::RAINY_PLAINS) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);
        return dis(gen) <= 15; // 15% chance to heal
    }
    return false;
}

bool Terrain::shouldReflect() const {
    return type == TerrainType::SKY_BASTION;
}

Terrain Terrain::generateRandomTerrain() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(TerrainType::RAINY_PLAINS));
    return Terrain(static_cast<TerrainType>(dis(gen)));
} 
#pragma once
#include <string>
#include <random>

enum class TerrainType {
    PLAINS,         // Normal terrain, no effects
    LAVA_FIELD,     // -2 health per turn
    ICY_VALLEY,     // 20% chance to miss attacks
    DARK_FOREST,    // -1 stamina per turn
    SAND_DUNES ,     // +1 stamina per turn
    MYSTIC,          // 50% chance to echo
    MOUNTAIN,       // +2 defense
    RAINY_PLAINS,    // 15% chance to heal 10 HP per turn
    SKY_BASTION
};

class Terrain {
private:
    TerrainType type;
    std::string name;
    std::string description;
    mutable std::mt19937 gen;

public:
    Terrain(TerrainType type);
    
    // Getters
    TerrainType getType() const;
    std::string getName() const;
    std::string getDescription() const;
    
    // Terrain effects
    int getHealthModifier() const;
    int getStaminaModifier() const;
    int getDefenseModifier() const;
    bool shouldMissAttack() const;
    bool shouldHeal() const;
    bool shouldEcho() const;
    bool shouldReflect() const;

    // Static terrain generation
    static Terrain generateRandomTerrain();
}; 
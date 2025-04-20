#include "../include/Blessing.h"
#include <random>
#include <ctime>
#include <iostream>

Blessing::Blessing() : name("No Blessing Equipped"), description(""), rarity(BlessingRarity::NONE), isActive(false) {}

Blessing::Blessing(const std::string& name, const std::string& description, BlessingRarity rarity)
    : name(name), description(description), rarity(rarity), isActive(false) {
}

std::string Blessing::getName() const {
    return name;
}

std::string Blessing::getDescription() const {
    return description;
}

BlessingRarity Blessing::getRarity() const {
    return rarity;
}

bool Blessing::isActiveBlessing() const {
    return isActive;
}



Blessing Blessing::generateKirikBlessing() {
    // Example kirik blessings
    static const std::vector<Blessing> kirikBlessings = {
        Blessing("Echo of Agony", "All status effects are activated twice", BlessingRarity::KIRIK),
        Blessing("Pierce the Veil", "Apply all status effects to the enemy", BlessingRarity::KIRIK)
    };
    
    return kirikBlessings[std::rand() % kirikBlessings.size()];
}

Blessing Blessing::generateBerxesBlessing() {
    // Example berxes blessings
    static const std::vector<Blessing> berxesBlessings = {
        Blessing("Titan's Wrath", "Your next attack deals triple damage, but you sacrifice 35 health", BlessingRarity::BERXES),
        Blessing("Crushing Momentum", "Next turn, you will heal for the amount of damage you deal", BlessingRarity::BERXES),
    };
    
    return berxesBlessings[std::rand() % berxesBlessings.size()];
}

Blessing Blessing::generateStadinBlessing() {
    // Example stadin blessings
    static const std::vector<Blessing> stadinBlessings = {
        Blessing("Stillness of Stone", "Add +20 defense for next 3 turns", BlessingRarity::STADIN),
        Blessing("Stadin Life", "Recover 50 health", BlessingRarity::STADIN),
    };
    
    return stadinBlessings[std::rand() % stadinBlessings.size()];
} 

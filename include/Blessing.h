#pragma once
#include <string>

enum class BlessingRarity {
    KIRIK,
    BERXES,
    STADIN,
    NONE
};

class Blessing {
private:
    std::string name;
    std::string description;
    BlessingRarity rarity;
    bool isActive;
    
public:
    Blessing();  // Default constructor
    Blessing(const std::string& name, const std::string& description, BlessingRarity rarity);
    
    std::string getName() const;
    std::string getDescription() const;
    BlessingRarity getRarity() const;
    bool isActiveBlessing() const;
    
   
    
    static Blessing generateKirikBlessing();
    static Blessing generateBerxesBlessing();
    static Blessing generateStadinBlessing();
}; 
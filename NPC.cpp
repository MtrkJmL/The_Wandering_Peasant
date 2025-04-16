#include "../include/NPC.h"

NPC::NPC(NPCType npcType) : type(npcType) {
    switch (type) {
        case NPCType::BLACKSMITH:
            name = "Master Smith";
            greeting = "Welcome to my forge! Need some improvements to your gear?";
            option1 = "Upgrade weapon (50 gold)";
            option2 = "Upgrade armor (50 gold)";
            response1 = "Your weapon should be more effective now!";
            response2 = "That armor looks much sturdier now!";
            break;
            
        case NPCType::MERCHANT:
            name = "Traveling Merchant";
            greeting = "Welcome to my shop! What can I get for you today?";
            option1 = "Buy health potion (50 gold)";
            option2 = "Buy upgrade scroll (200 gold)";
            response1 = "Here's your health potion!";
            response2 = "This scroll will improve your equipment!";
            break;
            
        case NPCType::AHMED:
            name = "Ahmed the Wise Tit";
            greeting = "Ah, a traveler! I can teach you many things.";
            option1 = "Learn about enemies";
            option2 = "Learn to bribe";
            response1 = "Let me teach you about these creatures...";
            response2 = "The art of bribery is a valuable skill...";
            break;
            
        default:
            name = "Unknown NPC";
            greeting = "...";
            option1 = "Leave";
            option2 = "Leave";
            response1 = "...";
            response2 = "...";
            break;
    }
} 
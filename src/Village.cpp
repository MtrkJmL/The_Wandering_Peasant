#include "../include/Village.h"
#include <iostream>
#include <algorithm>
#define NOMINMAX
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <limits>

Village::Village(const std::string& villageName) : name(villageName) {
    
}

void Village::enter(Player& player, Game& game) {
        std::cout << R"(
╔══════════════════════════════════════════════════════╗
║                  YOU FOUND A VILLAGE 🏘️              ║
╚══════════════════════════════════════════════════════╝

🌾 As you stride into the village, the dust of battle still clinging to your boots, a hush falls over the crowd.
Then—cheers erupt! 🎉 Children race beside you, elders nod with tearful smiles, and garlands are thrown at your feet.
The people, once trembling beneath the shadow of the Dragon King's servant, now look upon you as a hero of legend.
)";
        displayMenu(player);
    
}  

void Village::displayMenu(Player& player) {
    while (true) {
        std::cout << "\n--- Village of " << name << " ---" << std::endl;
        std::cout << "1. Visit Blacksmith" << std::endl;
        std::cout << "2. Visit Shrine" << std::endl;
        std::cout << "3. Visit Market" << std::endl;
        std::cout << "4. Visit Tavern" << std::endl;
        std::cout << "5. Exit Village" << std::endl;
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                visitBlacksmith(player);
                break;
            case 2:
                visitShrine(player);
                break;
            case 3:
                visitMarket(player);
                break;
            case 4:
                visitTavern(player);
                break;
            case 5:
                return;
            default:
                std::cout << "You wandered around the village for a while to see what you could do." << std::endl;
        }
    }
}

void Village::visitBlacksmith(Player& player) {
    if(Game::questManager.hasQuest("2") && Game::questManager.getStatus("2") == QuestStatus::TAKEN && !player.getMoltoTalked()){
            std::cout << R"(
    --- Molto Anvilgrin, the Eternal Banger ---
    Molto looks at the gem with a slight frown, his darkblood eyes glowing faintly. 
    'Ah, young one... I must confess, this task is beyond me. Darkbloods like myself cannot forge such gems. 
    It is... a *curse*, I suppose. But fear not, for I will guide you.'"
    )" << std::endl;

            std::cout << R"(
    Molto leans in closer, his voice lowering to a conspiratorial whisper.
    'Find a human blacksmith. One of those stout, sweaty fellows—always drenched in soot, 
    with an apron of humility and a belly full of ale. Tell them...' 
    )" << std::endl;

            std::cout << R"(
    'Molto Anvilgrin says you must forge this!'"
    )" << std::endl;

            std::cout << R"(
    Molto holds up a finger, cautioning you.
    'Now listen carefully. The blacksmith will likely try to dip it in molten metal—do not allow this!'
    His voice becomes more urgent.
    'No metal! No! What it needs... is molten lava. Hotter than any forge you’ve ever seen!'"
    )" << std::endl;

            std::cout << R"(
    Molto gives a sly grin and winks.
    'When they raise the hammer... you must stop them. No hammer! Tell them it must be shaped by hand. 
    Only then will the gem be ready.'"
    )" << std::endl;

            std::cout << R"(
    He leans back, tapping his foot with satisfaction.
    'Finally, give them five gold, but take back one... for pride’s sake. Too much gold, and they’ll lose their humility.'"
    )" << std::endl;

            std::cout << R"(
    Molto pauses, grinning.
    'And if the gem explodes, well... you’ll know you’ve done it right!'"
    )" << std::endl;

            std::cout << R"(
    With that, Molto turns to his forge, his hammer raised, ready to return to his eternal crafting.
    'Now go, young adventurer. May the fire of creation be with you.'"
    )" << std::endl;
        player.setMoltoTalked(true);
        }
    std::cout << "Gold: " << player.getGold() << std::endl;
    std::cout << "\n\n--- Molto Anvilgrin, the Eternal Banger ---" << std::endl;
    std::cout << "1. Upgrade Weapon(" << 300/player.getBriberySkill() << " gold)" << std::endl;
    std::cout << "2. Upgrade Armor(" << 300/player.getBriberySkill() << " gold)" << std::endl;   
    std::cout << "3. Exit Blacksmith" << std::endl;
    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            player.getWeapon().addModifier(2);
            player.spendGold(300/player.getBriberySkill());
            break;
        case 2:
            player.getArmor().addModifier(2);
            player.spendGold(300/player.getBriberySkill());
            break;
        case 3:
            return;
        default:
            std::cout << R"(You gaze too long at Molto Anvilgrin, the Eternal Banger — 
                    he senses your curiosity and promptly hurls you out, hammer-first.)" << std::endl;
    }
}

void Village::visitShrine(Player& player) {
    if (hasPrayed) {
        std::cout << "You've already received a blessing from this shrine.\n";
        return;
    }

    std::cout << R"(
    🛐 You enter the ancient shrine, lit dimly by divine glow...
    Three statues stand tall, each representing a forgotten champion of Zmar.
    
    Choose a champion to receive their blessing in exchange for a sacrifice:
    1. Kirik – Master of afflictions (max Stamina -4)
    2. Berxes – Brute force incarnate (max HP -40)
    3. Stadin – The eternal stillness (max HP -25 , max Stamina -2)
    4. Leave shrine
    )";
    std::cout << "Choice: ";
    int choice;
    std::cin >> choice;
    std::vector<Blessing> blessings;

    switch (choice) {
        case 1:
            player.setMaxStamina(-4);
            blessings.push_back(Blessing::generateKirikBlessing());
            do {
                blessings.push_back(Blessing::generateKirikBlessing());
            } while (blessings[1].getName() == blessings[0].getName());
            break;
        case 2:
            player.setMaxHealth(-40);
            blessings.push_back(Blessing::generateBerxesBlessing());
            do {
                blessings.push_back(Blessing::generateBerxesBlessing());
            } while (blessings[1].getName() == blessings[0].getName());
            break;
        case 3:
            player.setMaxHealth(-25);
            player.setMaxStamina(-2);
            blessings.push_back(Blessing::generateStadinBlessing());
            do {
                blessings.push_back(Blessing::generateStadinBlessing());
            } while (blessings[1].getName() == blessings[0].getName());
            break;
        case 4:
            std::cout << "You leave the shrine.\n";
            return;
        default:
            std::cout << "You hesitated... and the moment passed.\n";
            return;
    }

    // Prevent duplicate blessings
    if (blessings[0].getName() == blessings[1].getName()) {
        // Replace second with a new one if same
        if (choice == 1) blessings[1] = Blessing::generateKirikBlessing();
        else if (choice == 2) blessings[1] = Blessing::generateBerxesBlessing();
        else blessings[1] = Blessing::generateStadinBlessing();
    }

    std::cout << "\nChoose your blessing:\n";
    std::cout << "1. " << blessings[0].getName() << " – " << blessings[0].getDescription() << "\n";
    std::cout << "2. " << blessings[1].getName() << " – " << blessings[1].getDescription() << "\n";

    int blessChoice;
    std::cin >> blessChoice;

    if (blessChoice == 1 || blessChoice == 2) {
        Blessing chosen = blessings[blessChoice - 1];
        player.equipBlessing(chosen); // Save for use during combat
        hasPrayed = true;
        std::cout << "\n✨ You feel the essence of " << chosen.getName() << " course through your soul...\n";
    } else {
        std::cout << "You hesitated... the spirits faded.\n";
    }
}


void Village::visitMarket(Player& player) {
    int rerollCost = 100;
    bool inMarket = true;

    Item item1 = Item::generateRandomItem();
    Item item2 = Item::generateRandomItem();

    while (inMarket) {
        std::cout << "\nYou find a bustling market!\n";
        std::cout << "Your current gold: " << player.getGold() << "\n";

        std::cout << "\nItems for sale:\n";
        std::cout << "1. " << item1.getName() << " " << item1.getStatsString()
                  << " (Cost: " << item1.getValue() / player.getBriberySkill() << " gold, discount: "
                  << static_cast<int>((player.getBriberySkill() - 1.0f) * 100) << "%)\n";
        std::cout << "2. " << item2.getName() << " " << item2.getStatsString()
                  << " (Cost: " << item2.getValue() / player.getBriberySkill() << " gold, discount: "
                  << static_cast<int>((player.getBriberySkill() - 1.0f) * 100) << "%)\n";
        std::cout << "3. Reroll stocks (Cost: " << rerollCost << " gold)\n";
        std::cout << "4. Leave market\n";
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        Beep(200, 100);

        switch (choice) {
            case 1: {
                int price = item1.getValue() / player.getBriberySkill();
                if (player.spendGold(price)) {
                    if (item1.getType() == ItemType::WEAPON) {
                        player.equipWeapon(item1);
                    } else if (item1.getType() == ItemType::ARMOR) {
                        player.equipArmor(item1);
                    }
                    std::cout << "You purchase and equip the " << item1.getName() << "!\n";
                } else {
                    std::cout << "You don't have enough gold!\n";
                }
                break;
            }
            case 2: {
                int price = item2.getValue() / player.getBriberySkill();
                if (player.spendGold(price)) {
                    if (item2.getType() == ItemType::WEAPON) {
                        player.equipWeapon(item2);
                    } else if (item2.getType() == ItemType::ARMOR) {
                        player.equipArmor(item2);
                    }
                    std::cout << "You purchase and equip the " << item2.getName() << "!\n";
                } else {
                    std::cout << "You don't have enough gold!\n";
                }
                break;
            }
            case 3:
                if (player.getGold() >= rerollCost) {
                    player.spendGold(rerollCost);
                    item1 = Item::generateRandomItem();
                    item2 = Item::generateRandomItem();
                    std::cout << "You pay " << rerollCost << " gold and the merchant shows new items.\n";
                    rerollCost +=100;
                } else {
                    std::cout << "Not enough gold to reroll the shop!\n";
                }
                break;
            case 4:
                std::cout << "You leave the market.\n";
                inMarket = false;
                break;
            default:
                std::cout << "Invalid choice!\n";
                break;
        }
    }
        
}

void Village::visitTavern(Player& player) {
    std::cout << R"(

        🍺 You step into the tavern, greeted by crackling hearthfire and the cheerful hum of conversation.

        The scent of spiced stew and fresh bread fills the air,  
        mugs are raised, and a bard plays a gentle tune in the corner.

        This is a place of warmth, rest, and stories well told.

        )";
    bool inTavern = true;
    while (inTavern) {
        std::cout << "\n\n--- Tavern ---" << std::endl;
        std::cout << "1. Talk to the bartender" << std::endl;
        std::cout << "2. Challenge the bard to a duel" << std::endl;
        std::cout << "3. Talk to the patrons" << std::endl;
        std::cout << "4. Exit Tavern" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                talkToBartender(player);
                break;
            case 2:
                challengeBard(player);
                break;
            case 3:
                talkToNPCs(player);
                break;
            case 4:
                std::cout << "You leave the tavern." << std::endl;
                inTavern = false;
                break;
            default:
                std::cout << "You wandered around the tavern for a while to see what you could do." << std::endl;
        }
    }
}

void Village::talkToBartender(Player& player) {
    std::cout << "\nYou approach the bartender, a burly man wiping down mugs with a cloth.\n";

    // Bartender's greeting
    std::cout << "Bartender: 'What can I get ya? A drink, some food, or maybe a word of advice?' \n";

    // Dialogue options
    std::cout << "\n1. Ask about the local area\n";
    std::cout << "2. Buy a drink\n";
    std::cout << "3. Leave\n";

    int choice;
    std::cin >> choice;

    switch(choice) {
        case 1:
            {
                // Random bartender response about the area
                std::vector<std::string> areaInfo = {
                    "Bartender: 'Stay away from the Scorched Crater to the north. It's a dangerous place. Every step may cost you a life.'",
                    "Bartender: 'I heard an adventurer died in Stillwhite Valley because he slipped on a rock.'",
                    "Bartender: 'The Dark Forest to the west is a place of nightmares. I wouldn't go there if I were you.'",
                    "Bartender: 'The mountains to the east are said to be home to a powerful spirit. Many have tried to climb them, but none have returned.'",
                    "Bartender: 'The ruins to the south are said to be haunted. I wouldn't go there at night.'",
                    "Bartender: 'The Echoreach Glade to the north is a place of peace and reflection. It's said to be where Zmar's spirit wanders the world.'",
                    "Bartender: 'The Sand Dunes to the east are a place of beauty and danger. The warm winds can be deceiving. They may restore your stamina, but they can also be deadly.'",
                    "Bartender: 'The Summit of the Slain reminds and encourages every warrior that dragons can be defeated.'",
                    "Bartender: 'Since the war, The Rainy Plains to the south are a place of healing and danger. The rain may heal your wounds, but it can also be deadly.'",
                    "Bartender: 'No one knows where The Sky Bastion is. It is impossible to reach.'",
                    

                };
                

                srand(time(0));  // Initialize random seed
                int randomIndex = rand() % areaInfo.size();
                std::cout << areaInfo[randomIndex] << "\n";
            }
            break;
        case 2:
            {
                // Handle the purchase of a drink (add your logic for drinks)
                std::cout << "You buy a drink from the bartender. It’s a strong, bitter ale.\n";
                player.spendGold(5);  // Example: assume drinks cost 5 gold
                std::cout << "Your gold: " << player.getGold() << "\n";
            }
            break;
        case 3:
            std::cout << "You decide to leave the tavern.\n";
            break;
        default:
            std::cout << "The bartender looks at you quizzically as you stand there in silence.\n";
            break;
    }
}

void Village::challengeBard(Player& player) {
    NPC npc(NPCType::AHMED);
    std::cout << "A charismatic bard approaches you, lute in hand.\n";
    std::cout << "\"Fancy a musical challenge, traveler? Test your knowledge of songs!\"\n";
    std::cout << "\"How much gold are you willing to wager on your lyrical prowess?\"\n";

    int bet = 0;
    while (true) {
        std::cout << "Your gold: " << player.getGold() << std::endl;
        std::cout << "Enter your bet amount (or 0 to decline): ";
        std::cin >> bet;

        if (std::cin.fail()) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard bad input
            std::cout << "You attempt to wager trinkets and baubles, but the bard only laughs, 'These hold no value here, my friend.'\n";
        } else if (bet < 0) {
            std::cout << "You can't bet a negative amount!\n";
        } else if (bet == 0) {
            std::cout << "\"Perhaps another time then!\" The bard strums a chord and walks away.\n";
            return; // Player declined
        } else if (bet > player.getGold()) {
            std::cout << "\"Trying to bet more than you have, eh? Naughty, naughty!\" The bard winks.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard the rest of the line
            break; // Valid bet
        }
    }

    std::cout << "\n\"Excellent! A wager of " << bet << " gold!\"\n";
    std::cout << "\"Now, listen closely. Which of these tunes do you know?\"\n\n";

    // --- Generate 3 unique random song options ---
    std::vector<BardSong> allSongs;
    // Assuming BardSong enum values are contiguous from 0 to COUNT-1
    for (int i = 0; i <= static_cast<int>(BardSong::CHANT_OF_THE_MOONWATCHERS); ++i) {
        allSongs.push_back(static_cast<BardSong>(i));
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allSongs.begin(), allSongs.end(), g);

    std::vector<BardSong> options;
    options.push_back(allSongs[0]);
    options.push_back(allSongs[1]);
    options.push_back(allSongs[2]);
    // --- End song generation ---

    // --- Display options ---
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << i + 1 << ". " << npc.getBardSongName(options[i]) << std::endl;
    }
    // --- End display ---

    int choice = 0;
    while (true) {
        std::cout << "Enter the number of the song you choose: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
        } else if (choice < 1 || choice > 3) {
            std::cout << "Invalid choice. Please enter 1, 2, or 3.\n";
        } else {
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break; // Valid choice
        }
    }

    BardSong chosenSong = options[choice - 1];
    std::cout << "\nYou chose: " << npc.getBardSongName(chosenSong) << std::endl;
    std::cout << "Press Enter to start singing...\n\n" << std::endl;
    std::cin.get();
    

    // --- Check if player knows the song ---
    if (player.knowsBardSong(chosenSong)) {
        std::string verse = npc.getBardSongVerse(chosenSong);
        std::cout << "\"" << verse << "\"\n\n";  
        std::cout << "\n\"Ah, a connoisseur!\" the bard exclaims as you perfectly recount the melody.\n";
        std::cout << "\"You win the bet! Here's your prize.\"\n";
        player.addGold(bet);
        player.setBardBeaten(true);
        std::cout << "You gained " << bet << " gold.\n";
    } else {
        std::cout << "\nYou hum a few bars, but falter... The tune escapes you.\n";
        std::cout << "\"A valiant effort, but incorrect!\" the bard says sympathetically.\n";
        std::cout << "\"Better luck next time. The wager is mine.\"\n";
        player.spendGold(bet); // spendGold should return bool, but we assume it succeeds here based on prior checks
        std::cout << "You lost " << bet << " gold.\n";
    }

     std::cout << "Your current gold: " << player.getGold() << std::endl;
}

void Village::talkToNPCs(Player& player) {
    std::cout << R"(You approach the table with the confidence of a seasoned hero—only to be met with blank stares.
        A grizzled dwarf squints at you and mutters, "Table’s full... of people who bathe."
        Laughter follows, and someone silently slides a crusty potato toward you.
        It’s warm. It’s whispering.

        For a moment, you wonder if it's a relic… but no, it's even more useless than the fabled Stadin Relic.
        You leave with dignity. And the potato.)" << "\n";
}


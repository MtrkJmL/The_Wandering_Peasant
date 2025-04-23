#include "../include/Game.h"
#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <limits>
#include <fstream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <conio.h>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>


// Cross-platform implementation of _kbhit() for Unix-like systems
int _kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

// Cross-platform implementation of _getch() for Unix-like systems
int _getch() {
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    }
    return c;
}
#endif

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

Game::Game() : player("Hero"), isRunning(false), totalEnemiesDefeated(0), 
               encounterCount(0), isMiniBossEncounter(false), isFinalBossEncounter(false) {
    system("chcp 65001 > nul");
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}
QuestManager Game::questManager;  // Definition of the static member
void Game::run() {
    isRunning = true;
    
    displayMainMenu();
}

void playMainMenuMelody() {
    // Sequence of notes (Frequency in Hz, Duration in ms)
    Beep(523, 400);  // C4 - 523 Hz, 400 ms
    Beep(587, 200);  // D4 - 587 Hz, 400 ms
    Beep(659, 400);  // E4 - 659 Hz, 400 ms
    Beep(698, 200);  // F4 - 698 Hz, 400 ms
    Beep(523, 400);  // C4 - 523 Hz, 400 ms
    Beep(587, 200);  // D4 - 587 Hz, 400 ms
    Beep(659, 400);  // E4 - 659 Hz, 400 ms
    Beep(698, 200);  // F4 - 698 Hz, 400 ms
    Beep(784, 400);  // G4 - 784 Hz, 400 ms
    Beep(880, 800);  // A4 - 880 Hz, 400 ms
    Beep(988, 400);  // B4 - 988 Hz, 400 ms
    Beep(1047, 800); // C5 - 1047 Hz, 800 ms
    Beep(784, 100);  // G4 - 784 Hz, 400 ms
    Beep(659, 100);  // E4 - 659 Hz, 400 ms
    Beep(523, 600);  // C4 - 523 Hz, 400 ms
}

void Game::displayMainMenu() {
    while (isRunning) {
        std::cout << YELLOW << "\n=== 🌾🗡️ The Wandering Peasant 🔥🐉 ===\n"<< RESET;
        std::cout << "1. Start New Adventure\n";
        std::cout << "2. Exit\n";
        std::cout << "Choice: ";

        playMainMenuMelody();

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << RED << "Invalid input! I guess peasants don't know how to read.\n" << RESET;
            continue;
        }

        switch (choice) {
            case 1:
                startNewGame();
                break;
            case 2:
                isRunning = false;
                break;
            default:
                std::cout << RED << "That's why you are a peasant! Choose a valid option (1-2)!\n" << RESET;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        }
    }
}

void Game::startNewGame() {
    
    questManager.addQuest(Quest("1", "Bitter Ballads", "Ahmed seeks vengeance on his rival who stole his song and his spotlight — put an end to the egomaniac’s tune once and for all."));
    questManager.addQuest(Quest("2", "Banger of a Stone", "You’ve found a glowing stone with a note reading: 'Molto, here’s something for you to bang.' It’s time to find Molto and see what he can forge from this strange stone—who knows what chaos he’ll create."));
    questManager.addQuest(Quest("3", "He Who Stole My Boots", "An old, tattered advert nailed to a tree by the roadside, announcing that a man is urgently seeking a pair of boots. Description matches your Traveler's Boots. The reward for returning them may be far more than you bargained for."));
    questManager.addQuest(Quest("4", "Knightmare Navigation", "Help Sir G.P.Squire find his way back through the confusing lands of Peasaterra."));
    std::string playerName;
    std::cout << YELLOW << "\nYour peasant is called: " << RESET;
    std::cin >> playerName;
    
    player = Player(playerName);
    std::cout << playerName << " had a wonderful childhood.\n";
    std::cout << "\nWhen it came to hide-and-seek, none could match him. Some said it was skill, others... something more. He always triumphed, for he was:\n\n";
    std::cout << "1. ⚡" << BOLD << " Fleet of Foot " << RESET << "— “The wind itself envied his stride.” (+3 max stamina)\n";
    std::cout << "2. 🍀" << BOLD << " Blessed by Lady Luck " << RESET << "— “Dice rolled kindly, doors creaked just right, and shadows always favored him.” (+5 luck)\n";
    std::cout << "3. 💰" << BOLD << " Rich as Croesus " << RESET << "— “The gold coins clinked like a symphony of fortune.” (+250 gold)\n";
    std::cout << "4. 🤝" << BOLD << " Ally of the Underclans " << RESET << "— “His goblin friends were always looking out for him.” (Knows goblins)\n";
    std::cout << "5. 🪙" << BOLD << " Silver Tongue " << RESET << "— “With a wink, a coin, and a few carefully chosen words, he could sway even the most hardened heart. The art of persuasion was his to command.” (+2 level bribery skill)\n";
    std::cout << "6. ⚔️" << BOLD << " Forged in Fire " << RESET << "— “Before his first breath of adventure, he was given a blade that gleamed with the fire of a blacksmith’s craft and armor strong enough to turn aside a dragon’s breath.” (Better Starting Weapon and Armor)\n";
    std::cout << "7. 🏃‍♂️" << BOLD << " One Step Ahead " << RESET << "— “While others began their journey, he was already in motion, far beyond where they’d started. His early steps paved the way for greatness.” (Starts at Level 2)\n";
    std::cout << "8. 🏺" << BOLD << " Relic of the Past " << RESET << "— “A powerful relic rests in his hands, an heirloom of forgotten times, radiating mysterious energy. But alas, his armor is but humble peasant clothes, offering little protection.” (Starts with a rare relic, but only with peasant clothes)\n";
    std::cout << "9. 🕳️" << BOLD << " Cursed with Misfortune " << RESET << "— “Ah, my mistake. This little one was a loser. Every twig snapped, every sneeze betrayed him. Even the trees seemed to point him out.” (-10 luck)\n";
    std::cout << YELLOW << "Choose your past: " << RESET;
    int past;
    std::cin >> past;
    Beep(200,100);
    switch (past) {
    case 1:
        player.setMaxStamina(3);
        break;
    case 2:
        player.setLuck(5);
        break;
    case 3:
        player.addGold(250);
        break;
    case 4:
        player.learnEnemyType(EnemyType::GOBLIN);
        break;
    case 5:
        player.improveBriberySkill();
        player.improveBriberySkill();
        break;
    case 6:
        player.equipWeapon(Item(ItemType::WEAPON, "Less Rusty Sword", 1, 6, 4));
        player.equipArmor(Item(ItemType::ARMOR, "Chainmail", 1, 6, 2));
        break;
    case 7: 
        player.levelUp();
        player.addExperience(100);
        break;
    case 8:
        player.equipRelic(Relic::generateRareRelic());
        player.equipArmor(Item(ItemType::ARMOR, "Peasant Clothes", 0, 1, 1));
        break;
    case 9:
        player.setLuck(-10);
        break;
    }
    // Offer starting relic selection
    std::cout << "\n" << YELLOW << "Choose your starting relic: " << RESET << "\n";
    Relic relic1 = Relic::generateCommonRelic();
    Relic relic2 = Relic::generateCommonRelic();
    Relic relic3 = Relic::generateCommonRelic();
    
    std::cout << "1. " << relic1.getName() << " - " << relic1.getDescription() << "\n";
    std::cout << "2. " << relic2.getName() << " - " << relic2.getDescription() << "\n";
    std::cout << "3. " << relic3.getName() << " - " << relic3.getDescription() << "\n";
    std::cout << "4. Don't choose any\n";
    std::cout << YELLOW << "Choice: " << RESET;
    
    int choice;
    std::cin >> choice;
    Beep(200,100);
    switch (choice) {
        case 1: player.equipRelic(relic1); break;
        case 2: player.equipRelic(relic2); break;
        case 3: player.equipRelic(relic3); break;
        default:
            std::cout << "You decided to start with no relic or with whatever you already have from your past\n";
            break;
    }
    
    while (player.isAlive() && isRunning) {
        std::cout << YELLOW << "\n=== What will " << player.getName() << " do now? ===\n" << RESET;
        std::cout << "1. Wander around more\n";
        std::cout << "2. Rest\n";
        std::cout << "3. View Stats\n";
        std::cout << "4. Quit Game\n";
        std::cout << YELLOW << "Choice: " << RESET;

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << RED << "Invalid input! Please enter a number.\n" << RESET;
            continue;
        }
        Beep(200,100);
        switch (choice) {
            case 1:
                handleWandering();
                break;
            case 2:
                rest();
                break;
            case 3:
                displayPlayerStats();
                break;
            case 4:
                isRunning = false;
                break;
            default:
                std::cout << RED << "That's why you are a peasant! Choose a valid option (1-4)!\n" << RESET;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        }
    }

    if (!player.isAlive()) {
        gameOver();
    }
}

void Game::displayPlayerStats() {
    std::cout << "\n══════════════════════════════════════════════\n";
    std::cout << "         📜 " << BOLD << player.getName() << "'s Stats (Peasant) " << RESET << "         \n";
    std::cout << "══════════════════════════════════════════════\n";

    std::cout << "══════════════════╦═══════════════════════════\n";
    std::cout << " Level            ║ " << std::setw(24) << BOLD << player.getLevel() << RESET << " \n";
    std::cout << " Health           ║ " << std::setw(6) << BOLD << player.getHealth() 
              << " / " << std::setw(14) << BOLD << player.getMaxHealth() << RESET << " \n";
    std::cout << " Stamina          ║ " << std::setw(6) << BOLD << player.getStamina() 
              << " / " << std::setw(14) << BOLD << player.getMaxStamina() << RESET << " \n";
    std::cout << " Defense          ║ " << std::setw(6) << BOLD << player.getDefense() << RESET << " \n";
    std::cout << " Luck             ║ " << std::setw(6) << BOLD << player.getLuck() << RESET << " \n";
    std::cout << " Gold             ║ " << std::setw(24) << BOLD << player.getGold() << RESET << " \n";
    std::cout << " Experience       ║ " << std::setw(6) << BOLD << player.getExperience() 
              << " / " << std::setw(14) << BOLD << player.getExperienceToNextLevel() << RESET << " \n";
    std::cout << "══════════════════╩═══════════════════════════\n";

    std::cout << "\n" << YELLOW << "🛡️  EQUIPMENT" << RESET << "\n";
    std::cout << "══════════════════════════════════════════════\n";
    std::cout << " Weapon: " << std::setw(37) << std::left 
              << (player.getWeapon().getName() + " " + player.getWeapon().getStatsString()) << " \n";
    std::cout << " Armor : " << std::setw(37) << std::left 
              << (player.getArmor().getName() + " " + player.getArmor().getStatsString()) << " \n";
    std::cout << " Relic : " << std::setw(37) << std::left 
              << (player.getEquippedRelic().getName() + " - " + player.getEquippedRelic().getDescription()) << " \n";
    std::cout << "══════════════════════════════════════════════\n";

    std::cout << "\n" << YELLOW << "🧭 QUEST LOG" << RESET << "\n";
std::cout << "══════════════════════════════════════════════\n";
    std::cout << "📌 " << BLUE << "Active Quests:" << RESET << "\n";
    // Active quests
    questManager.showActiveQuests();

    std::cout << "\n" << GREEN << "✅ Completed Quests:" << RESET << "\n";
    questManager.showCompletedQuests();
        

    

std::cout << "\n══════════════════════════════════════════════\n";

    std::cout << "\n" << YELLOW << "🔸 Press Enter to continue..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    Beep(200,100);
}

void Game::handleWandering() {
    encounterCount++;
    
    // Check for special encounters
    if (encounterCount % 11 == 0) {
        isMiniBossEncounter = true;
    }
    if (encounterCount == 33) {
        isFinalBossEncounter = true;
    }
    
    if (isFinalBossEncounter) {
        handleFinalBoss();
        return;
    }
    
    if (isMiniBossEncounter) {
        handleMiniBoss();
        return;
    }
    
    // Regular branching path
    handleBranchingPath();
}

void Game::handleBranchingPath() {
    std::cout << "\nYou come to a fork in the road.\n";
    if(questManager.hasQuest("3") && questManager.getStatus("3") == QuestStatus::NOT_TAKEN && player.getEquippedRelic().getName() == "[🔵]Traveler's Mighty Boots"){
        std::cout << YELLOW << R"(
As you traverse the winding road, the sun beginning its slow descent behind the distant hills, something catches your eye. Tacked to a nearby tree is a hastily scrawled notice, fluttering slightly in the breeze. The ink is smeared in places, but the message is clear:

"LOST BOOTS — Reward Offered!"

A crude drawing of a boot accompanies the note, with the following written beneath:

_"A man of great discretion seeks a pair of sturdy boots to replace his. If you have any such footwear, please consider a trade. A substantial reward awaits those who aid in this quest."_

The note ends with an odd symbol, one that seems familiar yet distant, a mark of a clan you have yet to place.


)" << RESET << std::endl;
        questManager.takeQuest("3");

    }
    
    // Generate left path encounter
    Terrain leftTerrain = Terrain::generateRandomTerrain();
    int leftRoll = std::rand() % 100;
    std::string leftEncounter;
    EnemyType leftEnemyType;
    NPCType leftNPCType;
    if (leftRoll < 60) {
        // Enemy encounter
        leftEnemyType = static_cast<EnemyType>(std::rand() % 6);
        Enemy leftEnemy("Enemy", player.getLevel(), leftEnemyType);
        std::string enemyName;
        switch (leftEnemyType) {
            case EnemyType::GOBLIN: enemyName = "Goblin"; break;
            case EnemyType::ORC: enemyName = "Orc"; break;
            case EnemyType::TROLL: enemyName = "Troll"; break;
            case EnemyType::BANDIT: enemyName = "Bandit"; break;
            case EnemyType::SKELETON: enemyName = "Skeleton"; break;
            case EnemyType::DRAGON: enemyName = "Dragon"; break;
        }
        leftEncounter = "A level " + std::to_string(leftEnemy.getLevel()) + " " + enemyName + " at " + leftTerrain.getName() + " blocks your path";
    } else if (leftRoll < 75) {
        // NPC encounter
        leftNPCType = static_cast<NPCType>(std::rand() % 3);
        NPC leftNPC(leftNPCType);
        if (leftNPCType == NPCType::AHMED) {
            leftEncounter = "Ahmed the Wise Tit sits by a bonfire, ready to share his wisdom";
        } else if (leftNPCType == NPCType::BLACKSMITH) {
            leftEncounter = "A blacksmith works at his forge";
        } else {
            leftEncounter = "A merchant sets up his stall";
        }
    } else if (leftRoll < 90) {
        // Merchant encounter
        leftEncounter = "A merchant sets up his stall";
    } else {
        // Treasure
        leftEncounter = "A treasure chest glimmers in the distance";
    }
    
    // Generate right path encounter
    Terrain rightTerrain = Terrain::generateRandomTerrain();
    int rightRoll = std::rand() % 100;
    std::string rightEncounter;
    EnemyType rightEnemyType;
    NPCType rightNPCType;
    if (rightRoll < 60) {
        // Enemy encounter
        rightEnemyType = static_cast<EnemyType>(std::rand() % 6);
        Enemy rightEnemy("Enemy", player.getLevel(), rightEnemyType);
        std::string enemyName;
        switch (rightEnemyType) {
            case EnemyType::GOBLIN: enemyName = "Goblin"; break;
            case EnemyType::ORC: enemyName = "Orc"; break;
            case EnemyType::TROLL: enemyName = "Troll"; break;
            case EnemyType::BANDIT: enemyName = "Bandit"; break;
            case EnemyType::SKELETON: enemyName = "Skeleton"; break;
            case EnemyType::DRAGON: enemyName = "Dragon"; break;
        }
        rightEncounter = "A level " + std::to_string(rightEnemy.getLevel()) + " " + enemyName + " at " + rightTerrain.getName() + " patrols the area";
    } else if (rightRoll < 75) {
        // NPC encounter
        rightNPCType = static_cast<NPCType>(std::rand() % 3);
        NPC npc(rightNPCType);
        if (rightNPCType == NPCType::AHMED) {
            rightEncounter = "Ahmed the Wise Tit meditates under a tree";
        } else if (rightNPCType == NPCType::BLACKSMITH) {
            rightEncounter = "A blacksmith sharpens his tools";
        } else {
            rightEncounter = "A merchant displays his wares";
        }
    } else if (rightRoll < 90) {
        // Merchant encounter
        rightEncounter = "A merchant displays his wares";
    } else {
        // Treasure
        rightEncounter = "A treasure chest catches your eye, gleaming with the allure of hidden riches";
    }
    
    std::cout << "1. Go left (" << leftEncounter << ")\n";
    std::cout << "2. Go right (" << rightEncounter << ")\n";
    std::cout << "Choice: ";
    
    int choice;
    std::cin >> choice;
    Beep(200,100);
    if (choice == 1) {
        // Handle left path
        if (leftRoll < 60) {
            Enemy enemy("Enemy", player.getLevel(), leftEnemyType);
            handleEncounter(enemy, leftTerrain);
        } else if (leftRoll < 75) {
            handleNPCInteraction(leftNPCType);
        } else if (leftRoll < 90) {
            // Merchant encounter
            handleMarket();
        } else {
            int chestRoll = std::rand() % 100;
            if (chestRoll < 50) {
                int goldAmount = (std::rand() % 50 + 10) * player.getLevel();
                std::cout << "\nYou found " << goldAmount << " gold in the treasure chest!\n";
                player.addGold(goldAmount);
            } else {
                if(questManager.hasQuest("2") && questManager.getStatus("2") == QuestStatus::NOT_TAKEN){
                    std::cout << YELLOW << R"(
        You pry open the ancient treasure chest — the hinges groan like a dying beast.
        Inside, nestled among faded baubles and forgotten relics, lies a glowing stone pulsing with heat.
        A crumpled parchment rests beside it, scrawled with hasty ink:

            'Molto, here’s something worthy of your eternal banging.'

        The stone hums in your palm. Whoever this 'Molto' is, you sense the forge of fate is already burning.
    )" << RESET << std::endl;
                    
                        questManager.takeQuest("2");
                    }
                else{
                    int goldAmount = (std::rand() % 50 + 10) * player.getLevel() * 1.3;
                    std::cout << "\nYou are in luck! You found " << goldAmount << " gold in the treasure chest!\n";
                    player.addGold(goldAmount);

                }
            }
        }
    } else if (choice == 2) {
        // Handle right path
        if (rightRoll < 60) {
            Enemy enemy("Enemy", player.getLevel(), rightEnemyType);
            handleEncounter(enemy, rightTerrain);
        } else if (rightRoll < 75) {
            handleNPCInteraction(rightNPCType);
        } else if (rightRoll < 90) {
            handleMarket();
        } else {
            int chestRoll = std::rand() % 100;
            if(chestRoll < 50){
                int goldAmount = (std::rand() % 50 + 10) * player.getLevel();
                std::cout << "\nYou found " << goldAmount << " gold in the pile!\n";
                player.addGold(goldAmount);
            }
            else{
                if(questManager.hasQuest("4") && questManager.getStatus("4") == QuestStatus::NOT_TAKEN){
                    std::cout << YELLOW << R"(
You step forward, heart pounding with dreams of loot...

CLANG!

A figure crashes out from behind the chest in a whirl of dented steel and misplaced confidence.

A knight, if the term can stretch far enough, brandishes a sword with great ceremony (and very little control).

)" << RESET << std::endl;

    std::cout << YELLOW << "Sir G.P.Squire: “HALT, knave! By the authority of His Most Vague Majesty, I guard this chest!”\n" << RESET;
    std::cout << YELLOW << "“Declare thyself: Are you a royal courier—or a lowly treasure thief seeking to plunder the sacred chest of the kingdom?”\n\n" << RESET;

    std::cout << "1. \"I’m no thief! The king sent me to escort this chest!\"\n";
    std::cout << "2. \"If I were a thief, would I *walk* up to a knight with this chin?\"\n";
    std::cout << "Choice: ";

    int choice1;
    std::cin >> choice1;

    if (choice1 == 1) {
        std::cout << YELLOW << R"(
Sir G.P.Squire's eye twitches beneath his helm.

“HAH! I knew it! The courier lies dead behind that bush!”

He points with dramatic flair to a stiff, half-eaten corpse behind a nearby tree.

“You dare impersonate a royal courier?! This chest is sacred!”

Sir G.P.Squire attacks in the name of justice (and misplaced vengeance)!
)" << RESET << std::endl;
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    Beep(200,100);
    handleEncounter(Enemy("Sir G.P.Squire", player.getLevel()+5, EnemyType::SIR_G_P_SQUIRE), Terrain::generateRandomTerrain());

    } else {
        std::cout << YELLOW << R"(
Sir G.P.Squire lowers his blade just slightly, squinting at you.

“Hmph. Not the usual excuse… Very well. One more test.”

He clears his throat, standing proudly.

“Tell me, wanderer—what lies at the Summit of the Slain?”
)" << RESET << std::endl;

        std::cout << "1. \"Glory... and a name written in fire.\"\n";
        std::cout << "2. \"Probably a bunch of bird poop, honestly.\"\n";
        std::cout << "Choice: ";

        int choice2;
        std::cin >> choice2;

        if (choice2 == 1) {
            std::cout << YELLOW << R"(
Sir G.P.Squire gasps.

“That... that is exactly what the legends say. You speak as Kırık once did.”

He sheaths his sword with theatrical grace.

“Take this, wanderer. 'Tis the Featherstone Whistle. When you reach the Summit of the Slain... blow it.”

“I shall come. For glory, for answers, for..."

You blow the whistle, loudly and proudly.

He freezes.
His helmet slowly turns toward you.
A long sigh echoes from within.

"That... was a test blow, was it? Surely. Just a test..."
He stares at the sky for a moment, as if questioning every decision that led him here.

"No matter. My fate now lies in the hands of a peasant who whistles at random tree stumps."
He presses the whistle back into your palm.
"At the Summit of the Slain, you fool. Blow it there. Not here. Not now. Gods help me."

You vanish into the bushes without answering.
Somewhere in the distance, a hawk cries... perhaps in disappointment.
)" << RESET << std::endl;

            questManager.takeQuest("4");
        } else {
            std::cout << YELLOW << R"(
Sir G.P.Squire growls.

“Blasphemy! That summit is sacred, not a roost for pigeons!”

He raises his blade once more.

Prepare for a duel!
)" << RESET << std::endl;
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            Beep(200,100);
            handleEncounter(Enemy("Sir G.P.Squire", player.getLevel()+5, EnemyType::SIR_G_P_SQUIRE), Terrain::generateRandomTerrain());
        }
    }
                }
                else{
                    int goldAmount = (std::rand() % 50 + 10) * player.getLevel() * 1.3;
                    std::cout << "\nYou are in luck! You found " << goldAmount << " gold in the treasure chest!\n";
                    player.addGold(goldAmount);

                }
            }
            
        }
    } else {
        std::cout << "You wait at the crossroads in silence, and the sun rises on an unchosen path.\n";
    }
}

void Game::handleNPCInteraction(NPCType type) {
    switch (type) {
        case NPCType::BLACKSMITH:
            if(player.getMoltoTalked() && questManager.hasQuest("2") && questManager.getStatus("2") == QuestStatus::TAKEN){
                std::cout << YELLOW << R"(
    'Ah, a peasant seeking craftsmanship, eh?' he grins, his eyes gleaming with pride. 
    'What brings you to my humble forge, traveler?'

    You present the gem, explaining Molto’s instructions.

    The blacksmith raises an eyebrow, his grin widening. 
    'Molto, eh? He knows his way around a good forge. Alright, I'll do it. But I warn ye—this won't be a simple task.'
    )" << RESET << std::endl;
        char choice;
        std::cout << "This will cost you 1000 gold\n";
        std::cout << "Do you want to proceed? (y/n)\n";
        std::cout << "Choice: ";
        std::cin >> choice;
        Beep(200,100);
        if((choice == 'y' || choice == 'Y') && player.getGold() >= 1000){
            player.spendGold(1000);
            std::cout << YELLOW << R"(
    He motions for you to hand over the gem.
    'Give me a moment, lad/lass. I'll heat this to the right temperature. Just don’t go wanderin' off!'
    )" << RESET << std::endl;
        player.equipWeapon(Item(ItemType::WEAPON, "Molto's Banger of a Hammer", 4, 10, 0));
        questManager.completeQuest("2");
        std::cout << "\nYou gained 250 experience.\n" << std::endl;
        player.addExperience(250);
        }
        else{
            std::cout << YELLOW << R"(
    The blacksmith scowls, his hammer clanking hard against the anvil. 
    'Wastin’ my time, are ye? Come back when you've grown a spine!' 
    With a grunt, he shoves you out the door — soot, smoke, and all.
    )" << RESET << std::endl;
            return;
        }

        
        
            }
            std::cout << "\nBlacksmith: 'I can upgrade your equipment for a price.'\n";
            std::cout << "Your gold: " << player.getGold() << "\n";
            std::cout << "Your weapon: " << player.getWeapon().getName() << " " << player.getWeapon().getStatsString() << "\n";
            std::cout << "Your armor: " << player.getArmor().getName() << " " << player.getArmor().getStatsString() << "\n\n";
            std::cout << "1. Upgrade Weapon (" << static_cast<int>(100/player.getBriberySkill()) << " gold)\n";
            std::cout << "2. Upgrade Armor (" << static_cast<int>(100/player.getBriberySkill()) << " gold)\n";
            std::cout << "3. Leave\n";
            std::cout << "Choice: ";
            
            int blacksmithChoice;
            std::cin >> blacksmithChoice;
            Beep(200,100);
            switch (blacksmithChoice) {
                case 1:
                    if (player.getGold() >= static_cast<int>(100/player.getBriberySkill())) {
                        player.addGold(-static_cast<int>(100/player.getBriberySkill()));
                        player.getWeapon().addModifier(2);
                        std::cout << "Your weapon has been upgraded! Sharper than ever!\n";
                    } else {
                        std::cout << "Blacksmith raises an eyebrow. 'You're not carrying enough gold to upgrade your weapon!'\n";
                    }
                    break;
                case 2:
                    if (player.getGold() >= static_cast<int>(100/player.getBriberySkill())) {
                        player.addGold(-static_cast<int>(100/player.getBriberySkill()));
                        player.getArmor().addModifier(2);
                        std::cout << "Your armor has been upgraded! Sturdier than ever!\n";
                    } else {
                        std::cout << "Blacksmith raises an eyebrow. 'You're not carrying enough gold to upgrade your armor!'\n";
                    }
                    break;
                case 3:
                    std::cout << "Blacksmith: 'Come back when you need upgrades!'\n";
                    break;
                default:
                    std::cout << "Blacksmith doesn't know what you're trying to do! He kicks you out of his shop!\n";
                    break;
            }
            break;
        case NPCType::MERCHANT:
            std::cout << "Your gold: " << player.getGold() << "\n";
            std::cout << "Your weapon: " << player.getWeapon().getName() << " " << player.getWeapon().getStatsString() << "\n";
            std::cout << "Your armor: " << player.getArmor().getName() << " " << player.getArmor().getStatsString() << "\n\n";
            std::cout << "\nMerchant: 'I have some items for sale.'\n";
            std::cout << "1. Buy Health Potion (" << static_cast<int>(50/player.getBriberySkill()) << " gold)\n";
            std::cout << "2. Buy Weapon Upgrade Scroll (" << static_cast<int>(200/player.getBriberySkill()) << " gold)\n";
            std::cout << "3. Buy Armor Upgrade Scroll (" << static_cast<int>(200/player.getBriberySkill()) << " gold)\n";
            std::cout << "4. Leave\n";
            std::cout << "Choice: ";
            
            int choice;
            std::cin >> choice;
            Beep(200,100);
            switch (choice) {
                case 1:
                    if (player.getGold() >= static_cast<int>(50/player.getBriberySkill())) {
                        player.addGold(-static_cast<int>(50/player.getBriberySkill()));
                        player.heal(100);
                        std::cout << "You bought a health potion and restored 100 health!\n";
                    } else {
                        std::cout << "Not enough gold!\n";
                    }
                    break;
                case 2:
                    if (player.getGold() >= static_cast<int>(200/player.getBriberySkill())) {
                        player.addGold(-static_cast<int>(200/player.getBriberySkill()));
                        player.getWeapon().addDice();
                        std::cout << "You used the weapon upgrade scroll!\n";
                    } else {
                        std::cout << "Not enough gold!\n";
                    }
                    break;
                case 3:
                    if (player.getGold() >= static_cast<int>(200/player.getBriberySkill())) {
                        player.addGold(-static_cast<int>(200/player.getBriberySkill()));
                        player.getArmor().addModifier(1);
                        std::cout << "You used the armor upgrade scroll!\n";
                    } else {
                        std::cout << "Not enough gold!\n";
                    }
                    break;
                case 4:
                    std::cout << "Merchant: 'Come back when you need supplies!'\n";
                    break;
                default:
                    std::cout << "Invalid choice!\n";
                    break;
            }
            break;
        case NPCType::AHMED:
            NPC npc(NPCType::AHMED);
            if(player.getBardBeaten() && questManager.hasQuest("1") && questManager.getStatus("1") == QuestStatus::TAKEN){
                std::cout << YELLOW << R"(
                Ahmed gives a slow, knowing nod as you return.

                "So... the winds carried whispers of a humbled tune. 
                The braggart has been bested at last?"

                He closes his eyes, as if hearing a long-lost melody return to the air.  
                "For years, my verses were drowned beneath that fool's noise. 
                But now, the harmony is righted, the stage... at peace."

                He opens one eye and smirks.  
                "You’ve done a great service, not just for me — but for song itself."

                "May this aid your journey, and may your path be ever accompanied by worthy echoes."
                )" << RESET << std::endl;
                questManager.completeQuest("1");
                std::cout << "Your luck is increased by 30!\n\n";
                player.setLuck(30);
                std::cout << "\nYou gained 120 experience.\n" << std::endl;
                player.addExperience(120);

            }
            std::cout << "\nAhmed the Wise Tit: 'Ah, a traveler! I can teach you many things.'\n";
            std::cout << "1. Learn about enemies\n";
            std::cout << "2. Learn to bribe\n";
            std::cout << "3. Seek tales of the Bard's songs\n";
            std::cout << "4. Leave\n";
            std::cout << "Choice: ";
            
            std::cin >> choice;
            Beep(200,100);
            switch (choice) {
                case 1: {
                    std::cout << "\nWhich type of enemy would you like to learn about?\n";
                    std::cout << "1. Goblins\n";
                    std::cout << "2. Orcs\n";
                    std::cout << "3. Trolls\n";
                    std::cout << "4. Bandits\n";
                    std::cout << "5. Skeletons\n";
                    std::cout << "6. Dragons\n";
                    std::cout << "Choice: ";
                    
                    int enemyChoice;
                    std::cin >> enemyChoice;
                    
                    if (enemyChoice >= 1 && enemyChoice <= 5) {
                        EnemyType type = static_cast<EnemyType>(enemyChoice - 1);
                        player.learnEnemyType(type);
                        std::cout << "\nAhmed teaches you how to communicate with ";
                        switch (type) {
                            case EnemyType::GOBLIN: std::cout << "Goblins"; break;
                            case EnemyType::ORC: std::cout << "Orcs"; break;
                            case EnemyType::TROLL: std::cout << "Trolls"; break;
                            case EnemyType::BANDIT: std::cout << "Bandits"; break;
                            case EnemyType::SKELETON: std::cout << "Skeletons"; break;
                        }
                        std::cout << "!\n";
                    }else if (enemyChoice == 6) {
                        std::cout << "Speak with dragons? Hah... no mortal tongue can hold their words. Not even mine!\n";
                    } else {
                        std::cout << "Invalid choice!\n";
                    }
                    break;
                }
                case 2:
                    player.improveBriberySkill();
                    std::cout << "\nAhmed teaches you the art of bribery!\n";
                    std::cout << "Your bribery skill is now " << static_cast<int>(player.getBriberySkill() * 100) << "%!\n";
                    break;
                case 3:
                    if(questManager.getStatus("1") == QuestStatus::NOT_TAKEN){
                        
                        std::cout <<"\n\n" << YELLOW << R"(
    Ahmed: 'I will teach you the song you want. But first, I have a task for you. 
    There’s a pompous, spotlight-stealing charlatan prancing around as a bard—calls himself 'The Minstrel of Might.' 
    He stole my songs, my stage, and my dignity. I want you to find this egomaniac and challenge him to a bardic showdown. 
    Beat him at his own game, and I’ll make it worth your while. The tavern will sing of your glory—and I’ll finally sleep without rage in my heart.')" << RESET << std::endl;
                        questManager.takeQuest("1");
                        }
                    std::cout << "Which song would you like to hear?\n";
                    std::cout << "1. Ballad of Kirik\n";
                    std::cout << "2. Whispers of the Wyrm\n";
                    std::cout << "3. Eclipsefire Lament\n";
                    std::cout << "4. March of the Lost Crowns\n";
                    std::cout << "5. Dirge of the Darkbloods\n";
                    std::cout << "6. Hymns from the Hollow\n";
                    std::cout << "7. Tavern Tune of Glory\n";
                    std::cout << "8. Ode to the Last Flame\n";
                    std::cout << "9. Dance of the Starbound\n";
                    std::cout << "10. Chant of the Moonwatchers\n";
                    std::cout << "Choice: ";
                    int songChoice;
                    std::cin >> songChoice;
                    Beep(200,100);
                    switch (songChoice) {       
                        case 1:
                            std::cout << "\nAhmed sings the Ballad of Kirik:\n";
                            std::cout << npc.getBardSongVerse(BardSong::BALLAD_OF_KIRIK) << "\n";
                            player.learnBardSong(BardSong::BALLAD_OF_KIRIK);
                            break;
                        case 2:
                            std::cout << "\nAhmed sings the Whispers of the Wyrm:\n";
                            std::cout << npc.getBardSongVerse(BardSong::WHISPERS_OF_THE_WYRM) << "\n";
                            player.learnBardSong(BardSong::WHISPERS_OF_THE_WYRM);
                            break;
                        case 3:
                            std::cout << "\nAhmed sings the Eclipsefire Lament:\n";
                            std::cout << npc.getBardSongVerse(BardSong::ECLIPSEFIRE_LAMENT) << "\n";
                            player.learnBardSong(BardSong::ECLIPSEFIRE_LAMENT);
                            break;
                        case 4:
                            std::cout << "\nAhmed sings the March of the Lost Crowns:\n";     
                            std::cout << npc.getBardSongVerse(BardSong::MARCH_OF_THE_LOST_CROWNS) << "\n";
                            player.learnBardSong(BardSong::MARCH_OF_THE_LOST_CROWNS);
                            break;
                        case 5:
                            std::cout << "\nAhmed sings the Dirge of the Darkbloods:\n";
                            std::cout << npc.getBardSongVerse(BardSong::DIRGE_OF_THE_DARKBLOODS) << "\n";
                            player.learnBardSong(BardSong::DIRGE_OF_THE_DARKBLOODS);
                            break;
                        case 6:
                            std::cout << "\nAhmed sings the Hymns from the Hollow:\n";
                            std::cout << npc.getBardSongVerse(BardSong::HYMNS_FROM_THE_HOLLOW) << "\n"; 
                            player.learnBardSong(BardSong::HYMNS_FROM_THE_HOLLOW);
                            break;
                        case 7:
                            std::cout << "\nAhmed sings the Tavern Tune of Glory:\n";         
                            std::cout << npc.getBardSongVerse(BardSong::TAVERN_TUNE_OF_GLORY) << "\n";              
                            player.learnBardSong(BardSong::TAVERN_TUNE_OF_GLORY);
                            break;
                        case 8:
                            std::cout << "\nAhmed sings the Ode to the Last Flame:\n";
                            std::cout << npc.getBardSongVerse(BardSong::ODE_TO_THE_LAST_FLAME) << "\n";
                            player.learnBardSong(BardSong::ODE_TO_THE_LAST_FLAME);
                            break;
                        case 9:
                            std::cout << "\nAhmed sings the Dance of the Starbound:\n";
                            std::cout << npc.getBardSongVerse(BardSong::DANCE_OF_THE_STARBOUND) << "\n";
                            player.learnBardSong(BardSong::DANCE_OF_THE_STARBOUND);
                            break;
                        case 10:
                            std::cout << "\nAhmed sings the Chant of the Moonwatchers:\n";
                            std::cout << npc.getBardSongVerse(BardSong::CHANT_OF_THE_MOONWATCHERS) << "\n";
                            player.learnBardSong(BardSong::CHANT_OF_THE_MOONWATCHERS);
                            break;
                        default:
                            std::cout << "\nAhmed: 'Alas, that tune is beyond my knowledge.'\n";
                            break;
                    }
                    
                    break;
                case 4:
                    std::cout << "\nAhmed: 'Come back when you need wisdom!'\n";
                    break;
                default:
                    std::cout << "\nAhmed: 'Ah… I'm afraid your words are lost on these old ears.'\n";
                    break;
            }
            break;
    }
}

void Game::handleEncounter(Enemy& enemy, Terrain& terrain) {
    std::cout << "\nYou encounter a " << enemy.getName() << "!\n";
    std::cout << "Terrain: " << terrain.getName() << "\n";
    std::cout << "Terrain effect: " << terrain.getDescription() << "\n";
    player.startBattle();
    int bleedStack = 1;
    int reflectStack = 0;
    bool isActivated = false;
    int crushingMomentum = 3;
    bool berxesActive = false;
    bool kirikActive = false;
    bool stadinActive = false;
    int stillnessOfStone = 3;
    player.resetBlessingUsed();
    while (enemy.isAlive() && player.isAlive()) {
        // Clear screen and display battle UI
        std::cout << "\n\n";
        std::cout << BOLD << "╔════════════════════════════════════════════════╗\n";
        std::cout << "║              ⚔️  BATTLE COMMENCES  ⚔️          ║\n";
        std::cout << "╚════════════════════════════════════════════════╝\n\n" << RESET;
        std::cout << "  Terrain: " << terrain.getName() << "!\n";
        // Display player stats with hearts   
         std::cout << "  " << player.getName() << " (Level " << player.getLevel() << ")\n";
       
        std::cout << "  Health: ";
        int playerHearts = (player.getHealth() * 10) / player.getMaxHealth();
        for (int i = 0; i < 10; i++) {
            std::cout << (i < playerHearts ? "❤" : "  ");
        }
        std::cout << " " << player.getHealth() << "/" << player.getMaxHealth() << "\n";
        
        std::cout << "  Stamina: [";
        int playerStaminaBars = (player.getStamina() * 10) / player.getMaxStamina();
        for (int i = 0; i < 10; i++) {
            std::cout << (i < playerStaminaBars ? "⚡" : " ");
        }
        std::cout << "] " << player.getStamina() << "/" << player.getMaxStamina() << "\n";
        std::cout << "  Relic: " << player.getEquippedRelic().getName() << " - " << player.getEquippedRelic().getDescription() << "\n\n";
        
        // Display enemy stats with hearts
        
        std::string enemyName;
        switch (enemy.getType()) {
            case EnemyType::GOBLIN: enemyName = "Goblin"; break;
            case EnemyType::ORC: enemyName = "Orc"; break;
            case EnemyType::TROLL: enemyName = "Troll"; break;
            case EnemyType::BANDIT: enemyName = "Bandit"; break;
            case EnemyType::SKELETON: enemyName = "Skeleton"; break;
            case EnemyType::DRAGON: enemyName = "Dragon"; break;
            case EnemyType::SIR_G_P_SQUIRE: enemyName = "Sir G.P.Squire"; break;
        }
        
        
        std::cout << "  " << enemyName << " (Level " << enemy.getLevel() << ")\n";
        std::cout << "  Health: ";
        int enemyHearts = (enemy.getHealth() * 10) / enemy.getMaxHealth();
        for (int i = 0; i < 10; i++) {
            std::cout << (i < enemyHearts ? "❤" : "  ");
        }
        std::cout << " " << enemy.getHealth() << "/" << enemy.getMaxHealth() << "\n";
        if (enemy.isBleeding()) {
            std::cout << "  Status: " << RED << "🩸 Bleeding!" << RESET << "\n";
        }
        if (enemy.isBlinded()) {
            std::cout << "  Status: " << YELLOW << "❌👁 Blinded!" << RESET << "\n";
        }
        if (enemy.isPoisoned()) {
            std::cout << "  Status: " << GREEN << "☠️ Poisoned!" << RESET << "\n";
        }
        std::cout << "\n";
        
        std::cout << GREEN << "\n╔══════════════════════════════════════════════╗\n";
        std::cout <<   "║            ⚔️  YOUR TURN  ⚔️                 ║\n";
        std::cout <<   "╚══════════════════════════════════════════════╝\n" << RESET;
        std::cout << "Choose your action:\n";
        std::cout << "1. Light Attack (1 stamina) - Quick strike\n";
        std::cout << "2. Heavy Attack (2 stamina) - Powerful blow (may backfire)\n";
        std::cout << "3. Rest - Recover stamina\n";
        std::cout << "4. Talk - Try to communicate\n";
        std::cout << "5. Bribe (" << static_cast<int>(enemy.getBribeCost() / player.getBriberySkill()) << " gold)\n";
        std::cout << "6. Activate Blessing: " << player.getEquippedBlessing().getName() << "\n";
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        Beep(200,100);
        switch (choice) {
            case 1:
            case 2: {
                bool isHeavyAttack = (choice == 2);
                const int totalPositions = 16;
                int cursorPosition = 0;
                bool movingRight = true;
                bool keyPressed = false;
                char input;
                
                std::cout << "\nTime your attack! Press SPACE when the cursor is in the middle for maximum damage!\n";
                
                // Clear input buffer
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                // Set up non-blocking input
                #ifdef _WIN32
                    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
                    DWORD mode;
                    GetConsoleMode(hStdin, &mode);
                    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT) & (~ENABLE_LINE_INPUT));
                #else
                    struct termios oldt, newt;
                    tcgetattr(STDIN_FILENO, &oldt);
                    newt = oldt;
                    newt.c_lflag &= ~(ICANON | ECHO);
                    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
                #endif
                
                // Animation loop
                while (!keyPressed) {
                    // Clear previous line
                    std::cout << "\r[";
                    for (int i = 0; i < totalPositions; i++) {
                        if (i == cursorPosition) {
                            std::cout << "x";
                        } else if (i == totalPositions / 2) {
                            std::cout << "|"; // Mark the center
                        } else {
                            std::cout << " ";
                        }
                    }
                    std::cout << "]" << std::flush;
                    
                    // Check for input
                    if (_kbhit()) {
                        input = _getch();
                        if (input == ' ') {
                            keyPressed = true;
                        }
                    }
                    
                    // Move cursor
                    if (movingRight) {
                        cursorPosition++;
                        if (cursorPosition >= totalPositions - 1) {
                            movingRight = false;
                        }
                    } else {
                        cursorPosition--;
                        if (cursorPosition <= 0) {
                            movingRight = true;
                        }
                    }
                    
                    // Sleep for animation
                    std::this_thread::sleep_for(std::chrono::milliseconds(40/enemy.getLevel()));
                }
                Beep(150,200);
                // Restore terminal settings
                #ifdef _WIN32
                    SetConsoleMode(hStdin, mode);
                #else
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                #endif
                
                std::cout << "\n";
                if(terrain.shouldHeal()) {
                    player.heal(10);
                    std::cout << YELLOW << "As the soothing rains of the Whispering Thicket wash over you, your wounds begin to mend — nature grants you a moment of peace!\n" << RESET;
                    std::cout << YELLOW << "As you hear a whisper in the distance,tales of the ancient battle of the Dark Bloods echo through the trees... !\n" << RESET;
                    std::cout << YELLOW << "You regain 10 health!\n" << RESET;
                }
                bool isCrit = false;
                int damage = player.timingBasedAttack(cursorPosition, totalPositions, isHeavyAttack, isCrit);
                if(isCrit) {
                    reflectStack = 0;
                    if(terrain.shouldReflect()) {
                        std::cout << YELLOW << "Your strike lands true — a perfect blow that disrupts the Dragon King’s Eclipsefire Cataclysm, forcing him to recoil in fury!\n" << RESET;
                    }
                }
                if (terrain.shouldMissAttack()) {
                    std::cout << YELLOW << "You swing your weapon, but the treacherous ice beneath your feet betrays your footing — the strike goes wide, slicing only the cold air!\n" << RESET;
                    std::cout << YELLOW << "You lose your turn!\n" << RESET;
                    damage = 0;
                }
                if (damage > 0) {
                    if(berxesActive && player.getEquippedBlessing().getName() == "Titan's Wrath"){
                        damage *= 3;
                        player.takeDamage(35);
                        std::cout << MAGENTA << "Champion BERXES's ambition from battlefield trembles the earth with unyielding fury.\n" << RESET;
                        player.useBlessing();
                        berxesActive = false;
                    }
                    if(berxesActive && player.getEquippedBlessing().getName() == "Crushing Momentum"){
                        player.heal(damage);
                        std::cout << MAGENTA << "Champion BERXES's ambition from battlefield trembles the earth with unyielding fury.\n" << RESET;
                        player.useBlessing();
                        berxesActive = false;
                    }
                    enemy.takeDamage(damage);
                    std::cout << "\nYou strike with your " << player.getWeapon().getName() << "!\n";
                    std::cout << "Damage dealt: " << GREEN << damage << RESET << "\n";
                    if(terrain.shouldEcho()) {
                        std::cout << YELLOW << "\nThe terrain echoes with the sound of your strike, causing a weak echo!\n" << RESET;
                        enemy.takeDamage(damage/2);
                        std::cout << YELLOW << "The enemy takes " << damage/2 << " damage!\n" << RESET;
                    }
                    /*
                    // 20% chance to cause bleeding for light attack, 40% for heavy
                    if ((!isHeavyAttack && std::rand() % 100 < 20) || 
                        (isHeavyAttack && std::rand() % 100 < 40)) {
                        enemy.applyBleeding();
                        std::cout << "The enemy starts bleeding!\n";
                    }
                    */
                    static std::random_device rd;
                    static std::mt19937 gen(rd());
                    std::uniform_int_distribution<> chanceDis(1, 100);

                    StatusEffect effect = player.getWeapon().getStatusEffect();

                    if (chanceDis(gen) <= 20 + player.getLuck()) {  // 20% chance to apply any effect
                        switch (effect) {
                            case StatusEffect::BLEEDING:
                                if(player.getEquippedRelic().getName() == "[🔵]Reaper's Scythe") {
                                    bleedStack += 1;
                                }
                                enemy.applyBleeding();
                                std::cout << RED << "🩸 The enemy starts bleeding!\n" << RESET;
                                break;
                            case StatusEffect::POISON:
                                enemy.applyPoison();
                                std::cout << GREEN << "☠️ The enemy has been poisoned!\n" << RESET;
                                break;
                            case StatusEffect::BLINDNESS:
                                enemy.applyBlindness();
                                std::cout << YELLOW << "❌👁 The enemy is blinded!\n" << RESET;
                                break;
                            default:
                                break; // No effect
                        }
                    }

                    if(player.getEquippedRelic().getName() == "[🟠]KIRIK's Blessing") {
                        if (chanceDis(gen) <= 40 + player.getLuck()) {
                            enemy.applyBleeding();
                            enemy.applyPoison();
                            enemy.applyBlindness();
                            std::cout << BLUE << "ALMIGHTY KIRIK's Blessing activates! The enemy is now bleeding, poisoned, and blinded!\n" << RESET;
                            
                        }
                        
                    }
                }
                break;
            }
            case 3:
                player.rest();
                std::cout << "\nYou take a moment to catch your breath.\n";
                std::cout << "Stamina restored!\n";
                break;
            case 4:
                if (player.knowsEnemyType(enemy.getType())) {
                    std::cout << enemyName << ": '" << enemy.getDialogue() << "'\n";
                    std::cout << "The " << enemyName << " dropped their weapon and left.\n";
                    Item drop = enemy.generateDrop(player.getLuck());
                    std::cout << "\n" << enemyName << " dropped: " << drop.getName() << " " << drop.getStatsString() << "\n";
                    std::cout << "1. Take it (equip " << (drop.getType() == ItemType::WEAPON ? "weapon" : "armor") << ")\n";
                    std::cout << "2. Leave it (sell for " << drop.getValue() << " gold)\n";
                    std::cout << "Choice: ";
                    
                    std::cin >> choice;
                    Beep(200,100);
                    if (choice == 1) {
                        if (drop.getType() == ItemType::WEAPON) {
                            player.equipWeapon(drop);
                        } else {
                            player.equipArmor(drop);
                        }
                        std::cout << "You equip the " << drop.getName() << "!\n";
                    } else {
                        player.addGold(drop.getValue());
                        std::cout << "You sell the " << drop.getName() << " for " << drop.getValue() << " gold!\n";
                    }
                    return;
                } else {
                    std::cout <<"\n" << enemyName << " looks at you with a blank stare\n";
                }
                break;
            case 5: {
                int bribeCost = static_cast<int>(enemy.getBribeCost() / player.getBriberySkill());
                if (player.getGold() >= bribeCost) {  
                    if(enemy.getType() == EnemyType::DRAGON) {
                        std::cout << "'You dare barter with royalty?' it hisses. 'Your gold is dust compared to mine.'\n";
                        break;
                    } else {
                        player.addGold(-bribeCost);
                        std::cout << "You pay the " << enemyName << " " << bribeCost << " gold and it leaves.\n";
                    }
                    return;
                } else {
                    std::cout << "Not enough gold!\n";
                }
                break;
            }
            case 6: {
                if(player.hasUsedBlessing()){
                    std::cout << "You already activated your blessing!\n";
                    break;
                }
                std::cout << "You activate your " << MAGENTA << player.getEquippedBlessing().getName() << RESET << "!\n";
                if(player.getEquippedBlessing().getRarity() == BlessingRarity::KIRIK){
                    kirikActive = true;
                } else if(player.getEquippedBlessing().getRarity() == BlessingRarity::BERXES){
                    berxesActive = true;
                } else if(player.getEquippedBlessing().getRarity() == BlessingRarity::STADIN){
                    stadinActive = true;
                }
                if(player.getEquippedBlessing().getName() == "Stadin Life" && !player.hasUsedBlessing()){
                    player.heal(50);
                    std::cout << MAGENTA << "Champion Stadin's ambition from battlefield echoes through the skies.\n" << RESET;
                    std::cout << MAGENTA << "You regain 50 health!\n" << RESET;
                    player.useBlessing();
                }
                if (player.getEquippedBlessing().getName() == "Pierce the Veil" && !player.hasUsedBlessing()) {
                    
                    enemy.applyBleeding(); 
                    
                    enemy.applyPoison(); 
                     
                    enemy.applyBlindness(); 
                     


                    // Shuffle the vector to randomize the order
                    
                    std::cout << MAGENTA << "Champion KIRIK's ambition from battlefield cracks the silence of the heavens.\n" << RESET;
                    std::cout << MAGENTA << "The enemy is now bleeding, poisoned, and blinded!\n" << RESET;
                    player.useBlessing();
                }
                if (player.getEquippedBlessing().getName() == "Echo of Agony" && !player.hasUsedBlessing()){
                    player.useBlessing();
                }
                break;
            }
            default:
                std::cout << "Invalid choice! You hesitate and lose your turn.\n";
                break;
        }
        
        // Enemy's turn
            std::cout << RED << "\n╔══════════════════════════════════════════════╗\n";
            std::cout <<   "║            ⚔️  ENEMY'S TURN  ⚔️              ║\n";
            std::cout <<   "╚══════════════════════════════════════════════╝\n" << RESET;
        
         
        // Apply bleeding damage if enemy is bleeding
        if (enemy.isBleeding()) {
            for(int i = 0; i < bleedStack; i++) {   
                enemy.takeBleedingDamage();
                
                std::cout << RED << "Each movement deepens their wounds — blood drips to the earth below!" << enemyName << " takes 2 damage from bleeding!\n" << RESET;
                if(kirikActive && player.getEquippedBlessing().getName() == "Echo of Agony"){
                    enemy.takeBleedingDamage();
                    std::cout << MAGENTA << "Champion KIRIK's ambition from battlefield shakes the heavens with thunderous might.\n" << RESET;
                    std::cout << RED << enemyName << " takes 2 damage from bleeding!\n" << RESET;
                }
            }
        }
        int enemyRolled = enemy.rollAttack();
        int playerRolled = player.rollDefense();

        if(terrain.shouldReflect()) {
            if(reflectStack < 3) {
            std::cout << BOLD << "The Dragon King readies Eclipsefire Catacylsm! (" <<reflectStack <<"/3)\n" << RESET;
            reflectStack += 1;
            } else {
                std::cout << BOLD << "The Dragon King attacks with Eclipsefire Catacylsm! (" <<reflectStack <<" stacks)\n" << RESET;
                enemyRolled = 100;
                reflectStack = 0;
            }
        }

        if (enemy.isBlinded()) {
            enemy.blind();
            enemyRolled = 0;
            std::cout << YELLOW << "The enemy swings wildly, their eyes clouded. " << enemy.getName() << " strike finds no target!\n" << RESET;
        }
        
        if (enemy.isPoisoned()) {
            enemy.takePoisonDamage();
            if(kirikActive && player.getEquippedBlessing().getName() == "Echo of Agony"){
                std::cout << MAGENTA << "Champion KIRIK's ambition from battlefield shakes the heavens with thunderous might.\n" << RESET;
                enemy.takePoisonDamage();
            }
        }
        if(enemy.getHealth() <= 0) {
            std::cout << enemyName << " with a final gasp, staggers forward, as if to strike one last time...\n\n";
            enemyRolled = 0;
        }
        std::cout << enemyName << " rolled a " << enemyRolled << " and you rolled a " << playerRolled << ".\n";
        if(player.getEquippedRelic().getName() == "[⚪]Iron Heart") {
            playerRolled += 2;
            std::cout << BLUE << "Iron Heart activates!\n" << RESET;
        }
        if(terrain.getDefenseModifier() > 0) {
            playerRolled += terrain.getDefenseModifier();
            std::cout << YELLOW << "You feel the same ambition that once drove Lord KIRIK as he slew the ancient Dragon King on this summit.\n" << RESET;
            std::cout << YELLOW << "The weight of his triumph echoes through the summit, granting you +2 defense!\n" << RESET;
        }
        if(stadinActive && player.getEquippedBlessing().getName() == "Stillness of Stone" && stillnessOfStone > 0){
            playerRolled += 20;
            std::cout << MAGENTA << "Champion STADIN's ambition from battlefield echoes through the skies.\n" << RESET;
            stillnessOfStone -= 1;
        }
        int enemyDamage = std::max(0, enemyRolled - playerRolled);
        if(terrain.getHealthModifier() > 0) {
            player.takeDamage(terrain.getHealthModifier());
            std::cout << YELLOW << "As you try to hold your ground, the ground beneath you cracks, and the searing heat of the lava burns your skin-\n" << RESET;
            std::cout << YELLOW << "Because you are but a peasant, not a Dark Blood, causing you to lose " << terrain.getHealthModifier() << " health!\n" << RESET;
        }
        if (enemyDamage > 0) {
            player.takeDamage(enemyDamage);
            std::cout << enemyName << " lunges forward with a vicious slash!\n";
            std::cout << "Damage taken: " << RED << enemyDamage << RESET << "\n";
        } else {
            std::cout << "You raise your guard just in time — the attack glances off your weapon!\n";
        }

        if (!enemy.isAlive() && player.isAlive()) {
            std::cout << YELLOW << "\n╔══════════════════════════════════════════════╗\n";
            std::cout <<   "║            ⚔️  VICTORY ACHIEVED! ⚔️          ║\n";
            std::cout <<   "╚══════════════════════════════════════════════╝\n" << RESET;
            std::cout << "You defeated the " << enemyName << "!\n";
            Beep(880, 200);
            Beep(988, 200);
            Beep(1047, 300); 
            int goldReward = enemy.getGoldReward();
            int expReward = enemy.getExperienceReward();
            std::cout << "Rewards:\n";
            std::cout << "- Gold: " << goldReward << "\n";
            std::cout << "- Experience: " << expReward << "\n";
            player.addGold(goldReward);
            player.addExperience(expReward);
            
            // Handle item drops
            if (std::rand() % 100 < 35 + player.getLuck()) { // 35% chance for item drop
                Item drop = enemy.generateDrop(player.getLuck());
                std::cout << "\n" << enemy.getName() << " dropped: " << drop.getName() << " " << drop.getStatsString() << "\n";
                std::cout << "1. Take it (equip " << (drop.getType() == ItemType::WEAPON ? "weapon" : "armor") << ")\n";
                std::cout << "2. Leave it (sell for " << drop.getValue() << " gold)\n";
                std::cout << YELLOW << "Choice: " << RESET;
                
                std::cin >> choice;
                Beep(200,100);
                if (choice == 1) {
                    if (drop.getType() == ItemType::WEAPON) {
                        player.equipWeapon(drop);
                    } else {
                        player.equipArmor(drop);
                    }
                    std::cout << "You equip the " << drop.getName() << "!\n";
                } else {
                    player.addGold(drop.getValue());
                    std::cout << "You sell the " << drop.getName() << " for " << drop.getValue() << " gold!\n";
                }
            }
            if(terrain.getType() == TerrainType::MOUNTAIN && questManager.hasQuest("4") && questManager.getStatus("4") == QuestStatus::TAKEN){
                std::cout << YELLOW << R"(
    You raise the whistle to your lips and blow. The sound echoes across the summit, and within seconds, Sir G.P.Squire appears before you, his armor clanking with each step. 

    You can’t help but blink in surprise. 'Wait, how did you get here so quickly? It was only a moment after I blew the whistle!'

    Sir G.P.Squire, ever the confident knight, straightens his posture with a smug grin. 'Ah, simple. You see, I am Sir G.P.Squire—the master of finding the shortest path.' He taps his chest proudly. 'It’s in the name, after all!'

    He pauses for effect, looking at you with a knowing expression as if his explanation has just solved the greatest mystery of the land.

    'The shortest path?' you ask, still a bit confused.

    'Exactly!' He says, throwing his hands wide. 'I always know where to go. It’s a gift, really. Call it my *legendary sense of direction*.' He gives a wink, clearly proud of himself.

    'But you had no idea where Summit of the...', Sir G.P.Squire interrupts. 'Here is a reward for your efforts peasant! I don't need this sword anymore.'

    He hands you a sword and starts watching the sky.

    'I shall come. For glory, for answers, for...'

    'I shall come. For glory, for answers, for...'

    'I shall come. For glory, for answers, for...'
    )" << RESET;
            player.equipWeapon(Item(ItemType::WEAPON, "Glorious Path Seeker", 1, 40, player.getLuck()));
            questManager.completeQuest("4");
            player.addExperience(100);
            std::cout << "\nYou receive the Glorious Path Seeker!\n";
            std::cout << "\nYou gained 100 experience.\n" << std::endl;
            break;
            }
        }
        
        
        
        if (!player.isAlive()) {
            
            std::cout << RED << "\n========================================\n";
            std::cout << "            DEFEAT!                     \n";
            std::cout << "========================================\n" << RESET;
            std::cout << "You have been defeated by the " << enemyName << "!\n";
            Beep(220, 400);  // A3 - 220 Hz, 400 ms
            Beep(174, 400);  // F3 - 174 Hz, 400 ms
            Beep(130, 400);  // C3 - 130 Hz, 400 ms (lower tone for drama)
            Beep(100, 600);  // F2 - 100 Hz, 600 ms (slow final note for emphasis)
            break;
        }
        
        // Pause before next turn
        std::cout << "\nPress Enter to continue...\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        Beep(200,100);
    }
}

void Game::handleFinalBoss() {
    Enemy finalBoss("Dragon King", 15, EnemyType::DRAGON); // Level 15 boss
    std::cout << YELLOW << R"(
╔══════════════════════════════════════════════════════════════╗
║                       🔥 TWO TITANS MEET 🔥                  ║
╚══════════════════════════════════════════════════════════════╝

        A thunderous roar rends the heavens.
        The skies above burn a deep crimson...

          From the obsidian throne of flame,
          A colossal figure rises — wings outstretched,
          Crowned in fire, cloaked in shadow.

👑 Dragon King: "You have come far, mortal...
                But your journey ends here.
                Kneel... or be reduced to ash."

)" << RESET;
std::cout<< "1.Kneel before mighty Dragon King\n";
std::cout<< "2.Remember what he took from you, your dead wife and child, draw your weapon and charge!\n";
std::cout<< YELLOW << "Choice: " << RESET;
int choice;
std::cin>> choice;
Beep(200,100);
if(choice == 1) {
    std::cout<< YELLOW << "You kneel before the Dragon King, who smirks and says\n" << RESET;
    std::cout <<YELLOW << R"(
🐉 Dragon King: "Coward!"

His booming laughter echoes across the scorched skies.
He turns his back, wings unfurling with disdain.

🔥 Dragon King: "You are not worthy of flame nor fury.
                I will not stain my claws with your weakness."

He vanishes into the heavens, leaving only silence.

🗡 With heart heavy and hope extinguished,
    you fall to your knees, shame burning deeper than any wound.

In quiet resolve — you choose the final escape...

          💀 𝕐𝕠𝕦 𝕙𝕒𝕧𝕖 𝕗𝕒𝕝𝕝𝕖𝕟 𝕓𝕪 𝕪𝕠𝕦𝕣 𝕠𝕨𝕟 𝕙𝕒𝕟𝕕. 💀

)" << RESET;
    gameOver();
} else {
    std::cout << YELLOW << R"(
🔥 Your hand tightens around your weapon.
   You rise, defiance burning brighter than fear.

🐉 Dragon King: "Ah... so the lamb bares fangs."

His wings spread wide, casting a shadow that swallows the battlefield.

⚔️ You charge — heart pounding like war drums,
   steel singing your final vow.

💥 Flame meets flesh. Roar meets rage.
   Destiny ignites in a clash no bard will ever forget...

        ⚔️ 𝔸 𝕃𝔼𝔾𝔼ℕ𝔻 𝕀𝕊 𝔹𝕆ℝℕ 𝕀ℕ 𝔽𝕀ℝ𝔼 ⚔️

)" << RESET;
std::cout << "\n\nAs you charge, the ground beneath you shatters, and you are lifted into the sky!\n";
std::cout << "You are now on the Ashen Sky Bastion!\n";
std::cout << "Press Enter to start the final battle...\n";
std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
std::cin.get();
Beep(200,100);


    Terrain terrain = Terrain(TerrainType::SKY_BASTION);
    handleEncounter(finalBoss,terrain);
    
    if (finalBoss.isAlive()) {
        gameOver();
    } else {
        victory();
    }
    }
}

void Game::rest() {
    int healAmount = player.getMaxHealth() / 2;
    int restCost = 50;
    
    std::cout << player.getName() << " can rest to recover " << healAmount << " HP.\n";
    std::cout << "It will cost " << restCost << " gold.\n";
    std::cout << "Current gold: " << player.getGold() << "\n";
    
    if (player.getGold() < restCost) {
        std::cout << RED << "That's why you are a peasant! You can't afford to rest!\n" << RESET;
        return;
    }
    
    std::cout << "The path ahead is shrouded in mist. Will you take a short rest before continuing? (y/n)\n";
    char choice;
    std::cin >> choice;
    Beep(200,100);
    if (choice == 'y' || choice == 'Y') {
        if (player.spendGold(restCost)) {
            player.heal(healAmount);
            player.rest();  // This will recover stamina
            std::cout << player.getName() << " rests and recovers " << healAmount << " HP.\n";
            std::cout << "Current health: " << player.getHealth() << "/" << player.getMaxHealth() << "\n";
            std::cout << "Current stamina: " << player.getStamina() << "/" << player.getMaxStamina() << "\n";
        }
    } else {
        std::cout << player.getName() << " decides not to rest.\n";
    }
}

void Game::gameOver() {
    std::cout << "\n=== Game Over ===\n";
    std::cout << "You have been defeated!\n";
    std::cout << "Final Level: " << player.getLevel() << "\n";
    std::cout << "Final Gold: " << player.getGold() << "\n";
    isRunning = false;
}

void Game::victory() {
    std::cout << "\n========================================\n";
    std::cout << "           ✨ LEGEND FORGED ✨           \n";
    std::cout << "========================================\n";
    std::cout << "The final blow lands, echoing through the silent battlefield...\n";
    std::cout << "With a last breath, Dragon King crumbles — defeated not just by might,\n";
    std::cout << "but by the will of a peasant who would not fall.\n\n";
    std::cout << "The land is quiet now. Peace, once a dream, begins to bloom.\n";
    std::cout << "Songs will be sung of this day. Of your journey.\n";
    std::cout << "Of the choices you made, the lives you touched, the darkness you defied.\n\n";
    std::cout << "You are no longer just a peasant.\n";
    std::cout << "You are a legend.\n";
    std::cout << "========================================\n";
    std::cout << "         THANK YOU FOR PLAYING!         \n";
    std::cout << "========================================\n";
    isRunning = false;
}

void Game::showStatus() {
    std::cout << "\nStatus:\n";
    std::cout << "Name: " << player.getName() << "\n";
    std::cout << "Level: " << player.getLevel() << "\n";
    std::cout << "Health: " << player.getHealth() << "/" << player.getMaxHealth() << "\n";
    std::cout << "Stamina: " << player.getStamina() << "/" << player.getMaxStamina() << "\n";
    std::cout << "Gold: " << player.getGold() << "\n";
    std::cout << "Experience: " << player.getExperience() << "/" << player.getExperienceToNextLevel() << "\n";
    std::cout << "Total Enemies Defeated: " << totalEnemiesDefeated << "\n";
}

void Game::showInventory() {
    std::cout << "\nInventory:\n";
    std::cout << "Weapon: " << player.getWeapon().getName() << " " 
              << player.getWeapon().getStatsString() << "\n";
    std::cout << "Armor: " << player.getArmor().getName() << " " 
              << player.getArmor().getStatsString() << "\n";
    std::cout << "Gold: " << player.getGold() << "\n";
}

void Game::saveGame() {
    std::ofstream file("savegame.txt");
    if (file.is_open()) {
        file << player.getName() << "\n";
        file << player.getLevel() << "\n";
        file << player.getHealth() << "\n";
        file << player.getMaxHealth() << "\n";
        file << player.getGold() << "\n";
        file << player.getExperience() << "\n";
        file << totalEnemiesDefeated << "\n";
        file.close();
        std::cout << "Game saved successfully!\n";
    } else {
        std::cout << "Error saving game!\n";
    }
}

void Game::loadGame() {
    std::ifstream file("savegame.txt");
    if (file.is_open()) {
        std::string name;
        int level, health, maxHealth, gold, experience;
        
        file >> name >> level >> health >> maxHealth >> gold >> experience >> totalEnemiesDefeated;
        
        player = Player(name);
        // Set other stats...
        
        file.close();
        std::cout << "Game loaded successfully!\n";
    } else {
        std::cout << "No save game found!\n";
    }
}

void Game::handleMarket() {
    int rerollCost = 100;
    bool inMarket = true;

    Item item1 = Item::generateRandomItem();
    Item item2 = Item::generateRandomItem();

    while (inMarket) {
        std::cout << "\nYou find a bustling market!\n";
        std::cout << "Your current gold: " << player.getGold() << "\n";

        std::cout << "\nItems for sale:\n";
        std::cout << "1. " << item1.getName() << " " << item1.getStatsString()
                  << " (Cost: " << static_cast<int>(item1.getValue() / player.getBriberySkill()) << " gold, discount: "
                  << static_cast<int>((player.getBriberySkill() - 1.0f) * 100) << "%)\n";
        std::cout << "2. " << item2.getName() << " " << item2.getStatsString()
                  << " (Cost: " << static_cast<int>(item2.getValue() / player.getBriberySkill()) << " gold, discount: "
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
                    std::cout << "You don't have enough gold! (Need " << price << ")\n";
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
                    std::cout << "You don't have enough gold! (Need " << price << ")\n";
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

void Game::handleMiniBoss() {
    // Generate mini-boss
    Enemy miniBoss("Mini-Boss", player.getLevel() + encounterCount/7, EnemyType::DRAGON);
    std::cout << "The air thickens...\n";
    std::cout << "\nPress Enter to continue...\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    std::cout << "A hush falls over the field as the ground begins to shake...\n";
    std::cout << "\nPress Enter to continue...\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    std::cout << "\nA servant of the DRAGON KING appears!\n";
    Terrain terrain = Terrain::generateRandomTerrain();
    handleEncounter(miniBoss, terrain);
    
    if (!miniBoss.isAlive()) {
        // Generate relic options based on which mini-boss it is
        int miniBossNumber = encounterCount / 7;
        Relic relic1, relic2, relic3;
        auto rollRelic = [&](int commonThreshold, int rareThreshold) -> Relic {
            int roll = std::rand() % 100;
            if (roll < commonThreshold - player.getLuck()) {
                return Relic::generateCommonRelic();
            } else if (roll < rareThreshold) {
                return Relic::generateRareRelic();
            } else {
                return Relic::generateLegendaryRelic();
            }
        };

        
        if (miniBossNumber == 1) {
            // First mini-boss: 45% common, 40% rare, 15% legendary
            relic1 = rollRelic(45-player.getLuck(), 85);
            relic2 = rollRelic(45-player.getLuck(), 85);
            relic3 = rollRelic(45-player.getLuck(), 85);
        } else {
            // Second mini-boss: 50% rare, 25% common, 25% legendary
            relic1 = rollRelic(25-player.getLuck(), 75-player.getLuck());
            relic2 = rollRelic(25-player.getLuck(), 75-player.getLuck());
            relic3 = rollRelic(25-player.getLuck(), 75-player.getLuck());
        }
        
        std::cout << "\nThe mini-boss drops three relics! Choose one:\n";
        std::cout << "1. " << relic1.getName() << " - " << relic1.getDescription() << "\n";
        std::cout << "2. " << relic2.getName() << " - " << relic2.getDescription() << "\n";
        std::cout << "3. " << relic3.getName() << " - " << relic3.getDescription() << "\n";
        std::cout << "4. Keep your current relic\n";
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        Beep(200,100);
        switch (choice) {
            case 1: player.equipRelic(relic1); break;
            case 2: player.equipRelic(relic2); break;
            case 3: player.equipRelic(relic3); break;
            case 4: std::cout << "You keep your current relic.\n"; break;
            default:
                std::cout << "Invalid choice! You keep your current relic.\n";
                break;
        }
        Village village("Dragon's Summit");
        village.enter(player, *this);
    }
    
    isMiniBossEncounter = false;
} 
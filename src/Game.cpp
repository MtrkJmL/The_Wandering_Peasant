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

Game::Game() : player("Hero"), isRunning(false), totalEnemiesDefeated(0), 
               encounterCount(0), isMiniBossEncounter(false), isFinalBossEncounter(false) {
    system("chcp 65001 > nul");
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

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
        std::cout << "\n=== 🌾🗡️ The Wandering Peasant 🔥🐉 ===\n";
        std::cout << "1. Start New Adventure\n";
        std::cout << "2. Exit\n";
        std::cout << "Choice: ";

        playMainMenuMelody();

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input! I guess peasants don't know how to read.\n";
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
                std::cout << "That's why you are a peasant! Choose a valid option (1-2)!\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        }
    }
}

void Game::startNewGame() {
    std::string playerName;
    std::cout << "Your peasant is called: ";
    std::cin >> playerName;
    
    player = Player(playerName);
    std::cout << playerName << " had a wonderful childhood.\n";
    std::cout << "When it came to hide-and-seek, none could match him. Some said it was skill, others... something more. He always triumphed, for he was:\n";
    std::cout << "1. ⚡ Fleet of Foot — “The wind itself envied his stride.” (+3 max stamina)\n";
    std::cout << "2. 🍀 Blessed by Lady Luck — “Dice rolled kindly, doors creaked just right, and shadows always favored him.” (+10 luck)\n";
    std::cout << "3. 🕳️ Cursed with Misfortune — “Ah, my mistake. This little one was a loser. Every twig snapped, every sneeze betrayed him. Even the trees seemed to point him out.” (-10 luck)\n";
    std::cout << "Choose your past:";
    int past;
    std::cin >> past;
    Beep(200,100);
    if (past == 1) {
        player.setMaxStamina(3);
    } else if (past == 2) {
        player.setLuck(10);
    } else {
        player.setLuck(-10);
    }
    // Offer starting relic selection
    std::cout << "\nChoose your starting relic:\n";
    Relic relic1 = Relic::generateCommonRelic();
    Relic relic2 = Relic::generateCommonRelic();
    Relic relic3 = Relic::generateCommonRelic();
    
    std::cout << "1. " << relic1.getName() << " - " << relic1.getDescription() << "\n";
    std::cout << "2. " << relic2.getName() << " - " << relic2.getDescription() << "\n";
    std::cout << "3. " << relic3.getName() << " - " << relic3.getDescription() << "\n";
    std::cout << "Choice: ";
    
    int choice;
    std::cin >> choice;
    Beep(200,100);
    switch (choice) {
        case 1: player.equipRelic(relic1); break;
        case 2: player.equipRelic(relic2); break;
        case 3: player.equipRelic(relic3); break;
        default:
            std::cout << "Invalid choice! As a punishment, you start with no relic.\n";
            break;
    }
    
    while (player.isAlive() && isRunning) {
        std::cout << "\n=== What will " << player.getName() << " do now? ===\n";
        std::cout << "1. Wander around more\n";
        std::cout << "2. Rest\n";
        std::cout << "3. View Stats\n";
        std::cout << "4. Quit Game\n";
        std::cout << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input! Please enter a number.\n";
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
                std::cout << "That's why you are a peasant! Choose a valid option (1-4)!\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        }
    }

    if (!player.isAlive()) {
        gameOver();
    }
}

void Game::displayPlayerStats() {
    std::cout << "\n=== " << player.getName() << "'s Stats (as a peasant) ===\n";
    std::cout << "Level: " << player.getLevel() << "\n";
    std::cout << "Health: " << player.getHealth() << "/" << player.getMaxHealth() << "\n";
    std::cout << "Stamina: " << player.getStamina() << "/" << player.getMaxStamina() << "\n";
    std::cout << "Gold: " << player.getGold() << "\n";
    std::cout << "Experience: " << player.getExperience() << "/" << player.getExperienceToNextLevel() << "\n";
    
    std::cout << "\nEquipment:\n";
    std::cout << "Weapon: " << player.getWeapon().getName() << " " << player.getWeapon().getStatsString() << "\n";
    std::cout << "Armor: " << player.getArmor().getName() << " " << player.getArmor().getStatsString() << "\n";
    std::cout << "Relic: " << player.getEquippedRelic().getName() << " " << player.getEquippedRelic().getDescription() << "\n";

    std::cout << "\nPress Enter to continue...\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void Game::handleWandering() {
    encounterCount++;
    
    // Check for special encounters
    if (encounterCount % 7 == 0) {
        isMiniBossEncounter = true;
    }
    if (encounterCount == 21) {
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
        rightEncounter = "A pile of gold coins catches your eye";
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
            int goldAmount = (std::rand() % 50 + 10) * player.getLevel();
            std::cout << "\nYou found " << goldAmount << " gold in the treasure chest!\n";
            player.addGold(goldAmount);
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
            int goldAmount = (std::rand() % 50 + 10) * player.getLevel();
            std::cout << "\nYou found " << goldAmount << " gold in the pile!\n";
            player.addGold(goldAmount);
        }
    } else {
        std::cout << "You wait at the crossroads in silence, and the sun rises on an unchosen path.\n";
    }
}

void Game::handleNPCInteraction(NPCType type) {
    switch (type) {
        case NPCType::BLACKSMITH:
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
                            std::cout << "Ahmed sings the Ballad of Kirik:\n";
                            std::cout << npc.getBardSongVerse(BardSong::BALLAD_OF_KIRIK) << "\n";
                            break;
                        case 2:
                            std::cout << "Ahmed sings the Whispers of the Wyrm:\n";
                            std::cout << npc.getBardSongVerse(BardSong::WHISPERS_OF_THE_WYRM) << "\n";
                            break;
                        case 3:
                            std::cout << "Ahmed sings the Eclipsefire Lament:\n";
                            std::cout << npc.getBardSongVerse(BardSong::ECLIPSEFIRE_LAMENT) << "\n";
                            break;
                        case 4:
                            std::cout << "Ahmed sings the March of the Lost Crowns:\n";     
                            std::cout << npc.getBardSongVerse(BardSong::MARCH_OF_THE_LOST_CROWNS) << "\n";
                            break;
                        case 5:
                            std::cout << "Ahmed sings the Dirge of the Darkbloods:\n";
                            std::cout << npc.getBardSongVerse(BardSong::DIRGE_OF_THE_DARKBLOODS) << "\n";
                            break;
                        case 6:
                            std::cout << "Ahmed sings the Hymns from the Hollow:\n";
                            std::cout << npc.getBardSongVerse(BardSong::HYMNS_FROM_THE_HOLLOW) << "\n";
                            break;
                        case 7:
                            std::cout << "Ahmed sings the Tavern Tune of Glory:\n";         
                            std::cout << npc.getBardSongVerse(BardSong::TAVERN_TUNE_OF_GLORY) << "\n";              
                            break;
                        case 8:
                            std::cout << "Ahmed sings the Ode to the Last Flame:\n";
                            std::cout << npc.getBardSongVerse(BardSong::ODE_TO_THE_LAST_FLAME) << "\n";
                            break;
                        case 9:
                            std::cout << "Ahmed sings the Dance of the Starbound:\n";
                            std::cout << npc.getBardSongVerse(BardSong::DANCE_OF_THE_STARBOUND) << "\n";
                            break;
                        case 10:
                            std::cout << "Ahmed sings the Chant of the Moonwatchers:\n";
                            std::cout << npc.getBardSongVerse(BardSong::CHANT_OF_THE_MOONWATCHERS) << "\n";
                            break;
                        default:
                            std::cout << "Ahmed: 'Alas, that tune is beyond my knowledge.'\n";
                            break;
                    }
                    break;
                case 4:
                    std::cout << "Ahmed: 'Come back when you need wisdom!'\n";
                    break;
                default:
                    std::cout << "Ahmed: 'Ah… I'm afraid your words are lost on these old ears.'\n";
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
        std::cout << "╔════════════════════════════════════════════════╗\n";
        std::cout << "║              ⚔️  BATTLE COMMENCES  ⚔️          ║\n";
        std::cout << "╚════════════════════════════════════════════════╝\n\n";
        std::cout << "Terrain: " << terrain.getName() << "!\n";
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
        }
        
        std::cout << "  " << enemyName << " (Level " << enemy.getLevel() << ")\n";
        std::cout << "  Health: ";
        int enemyHearts = (enemy.getHealth() * 10) / enemy.getMaxHealth();
        for (int i = 0; i < 10; i++) {
            std::cout << (i < enemyHearts ? "❤" : "  ");
        }
        std::cout << " " << enemy.getHealth() << "/" << enemy.getMaxHealth() << "\n";
        if (enemy.isBleeding()) {
            std::cout << "  Status: 🩸 Bleeding!\n";
        }
        if (enemy.isBlinded()) {
            std::cout << "  Status: ❌👁 Blinded!\n";
        }
        if (enemy.isPoisoned()) {
            std::cout << "  Status: ☠️ Poisoned!\n";
        }
        std::cout << "\n";
        
        std::cout << "\n╔══════════════════════════════════════════════╗\n";
        std::cout <<   "║            ⚔️  YOUR TURN  ⚔️                 ║\n";
        std::cout <<   "╚══════════════════════════════════════════════╝\n";
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
                    std::this_thread::sleep_for(std::chrono::milliseconds(25/enemy.getLevel()));
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
                    std::cout << "As the soothing rains of the Whispering Thicket wash over you, your wounds begin to mend — nature grants you a moment of peace!\n";
                    std::cout << "As you hear a whisper in the distance,tales of the ancient battle of the Dark Bloods echo through the trees... !\n";
                    std::cout << "You regain 10 health!\n";
                }
                bool isCrit = false;
                int damage = player.timingBasedAttack(cursorPosition, totalPositions, isHeavyAttack, isCrit);
                if(isCrit) {
                    reflectStack = 0;
                    if(terrain.shouldReflect()) {
                        std::cout << "Your strike lands true — a perfect blow that disrupts the Dragon King’s Eclipsefire Cataclysm, forcing him to recoil in fury!\n";
                    }
                }
                if (terrain.shouldMissAttack()) {
                    std::cout << "You swing your weapon, but the treacherous ice beneath your feet betrays your footing — the strike goes wide, slicing only the cold air!\n";
                    std::cout << "You lose your turn!\n";
                    damage = 0;
                }
                if (damage > 0) {
                    if(berxesActive && player.getEquippedBlessing().getName() == "Titan's Wrath"){
                        damage *= 3;
                        player.takeDamage(20);
                        std::cout << "Champion BERXES's ambition from battlefield trembles the earth with unyielding fury.\n";
                        player.useBlessing();
                    }
                    if(berxesActive && player.getEquippedBlessing().getName() == "Crushing Momentum"){
                        player.heal(damage);
                        std::cout << "Champion BERXES's ambition from battlefield trembles the earth with unyielding fury.\n";
                        player.useBlessing();
                    }
                    enemy.takeDamage(damage);
                    std::cout << "\nYou strike with your " << player.getWeapon().getName() << "!\n";
                    std::cout << "Damage dealt: " << damage << "\n";
                    if(terrain.shouldEcho()) {
                        std::cout << "The terrain echoes with the sound of your strike, causing a weak echo!\n";
                        enemy.takeDamage(damage/2);
                        std::cout << "The enemy takes " << damage/2 << " damage!\n";
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
                                std::cout << "🩸 The enemy starts bleeding!\n";
                                break;
                            case StatusEffect::POISON:
                                enemy.applyPoison();
                                std::cout << "☠️ The enemy has been poisoned!\n";
                                break;
                            case StatusEffect::BLINDNESS:
                                enemy.applyBlindness();
                                std::cout << "❌👁 The enemy is blinded!\n";
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
                            std::cout << "ALMIGHTY KIRIK's Blessing activates! The enemy is now bleeding, poisoned, and blinded!\n";
                            
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
                    std::cout << enemy.getName() << ": '" << enemy.getDialogue() << "'\n";
                    std::cout << "The " << enemy.getName() << " dropped their weapon and left.\n";
                    Item drop = enemy.generateDrop();
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
                    std::cout <<"\n" << enemy.getName() << " looks at you with a blank stare\n";
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
                        std::cout << "You pay the " << enemy.getName() << " " << bribeCost << " gold and it leaves.\n";
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
                std::cout << "You activate your " << player.getEquippedBlessing().getName() << "!\n";
                if(player.getEquippedBlessing().getRarity() == BlessingRarity::KIRIK){
                    kirikActive = true;
                } else if(player.getEquippedBlessing().getRarity() == BlessingRarity::BERXES){
                    berxesActive = true;
                } else if(player.getEquippedBlessing().getRarity() == BlessingRarity::STADIN){
                    stadinActive = true;
                }
                if(player.getEquippedBlessing().getName() == "Stadin Life" && !player.hasUsedBlessing()){
                    player.heal(50);
                    std::cout << "Champion Stadin's ambition from battlefield echoes through the skies.\n";
                    std::cout << "You regain 50 health!\n";
                    player.useBlessing();
                }
                if (player.getEquippedBlessing().getName() == "Pierce the Veil" && !player.hasUsedBlessing()) {
                    
                    enemy.applyBleeding(); 
                    
                    enemy.applyPoison(); 
                     
                    enemy.applyBlindness(); 
                     


                    // Shuffle the vector to randomize the order
                    
                    std::cout << "Champion KIRIK's ambition from battlefield cracks the silence of the heavens.\n";
                    std::cout << "The enemy is now bleeding, poisoned, and blinded!\n";
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
            std::cout << "\n╔══════════════════════════════════════════════╗\n";
            std::cout <<   "║            ⚔️  ENEMY'S TURN  ⚔️              ║\n";
            std::cout <<   "╚══════════════════════════════════════════════╝\n";
        
        // Apply bleeding damage if enemy is bleeding
        if (enemy.isBleeding()) {
            for(int i = 0; i < bleedStack; i++) {   
                enemy.takeBleedingDamage();
                
                std::cout << "Each movement deepens their wounds — blood drips to the earth below!" << enemyName << " takes 2 damage from bleeding!\n";
                if(kirikActive && player.getEquippedBlessing().getName() == "Echo of Agony"){
                    enemy.takeBleedingDamage();
                    std::cout << "Champion KIRIK's ambition from battlefield shakes the heavens with thunderous might.\n";
                    std::cout << enemyName << " takes 2 damage from bleeding!\n";
                }
            }
        }
        int enemyRolled = enemy.rollAttack();
        int playerRolled = player.rollDefense();

        if(terrain.shouldReflect()) {
            if(reflectStack < 3) {
            std::cout << "The Dragon King readies Eclipsefire Catacylsm! (" <<reflectStack <<"/3)\n";
            reflectStack += 1;
            } else {
                std::cout << "The Dragon King attacks with Eclipsefire Catacylsm! (" <<reflectStack <<" stacks)\n";
                enemyRolled = 100;
                reflectStack = 0;
            }
        }

        if (enemy.isBlinded()) {
            enemy.blind();
            enemyRolled = 0;
            std::cout << "The enemy swings wildly, their eyes clouded. " << enemyName << " strike finds no target!\n";
        }
        
        if (enemy.isPoisoned()) {
            enemy.takePoisonDamage();
            if(kirikActive && player.getEquippedBlessing().getName() == "Echo of Agony"){
                std::cout << "Champion KIRIK's ambition from battlefield shakes the heavens with thunderous might.\n";
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
            std::cout << "Iron Heart activates!\n";
        }
        if(terrain.getDefenseModifier() > 0) {
            playerRolled += terrain.getDefenseModifier();
            std::cout << "You feel the same ambition that once drove Lord KIRIK as he slew the ancient Dragon King on this summit.\n";
            std::cout << "The weight of his triumph echoes through the summit, granting you +2 defense!\n";
        }
        if(stadinActive && player.getEquippedBlessing().getName() == "Stillness of Stone" && stillnessOfStone > 0){
            playerRolled += 20;
            std::cout << "Champion STADIN's ambition from battlefield echoes through the skies.\n";
            stillnessOfStone -= 1;
        }
        int enemyDamage = std::max(0, enemyRolled - playerRolled);
        if(terrain.getHealthModifier() > 0) {
            player.takeDamage(terrain.getHealthModifier());
            std::cout << "As you try to hold your ground, the ground beneath you cracks, and the searing heat of the lava burns your skin-\n";
            std::cout << "Because you are but a peasant, not a Dark Blood, causing you to lose " << terrain.getHealthModifier() << " health!\n";
        }
        if (enemyDamage > 0) {
            player.takeDamage(enemyDamage);
            std::cout << enemyName << " lunges forward with a vicious slash!\n";
            std::cout << "Damage taken: " << enemyDamage << "\n";
        } else {
            std::cout << "You raise your guard just in time — the attack glances off your weapon!\n";
        }

        if (!enemy.isAlive() && player.isAlive()) {
            std::cout << "\n╔══════════════════════════════════════════════╗\n";
            std::cout <<   "║            ⚔️  VICTORY ACHIEVED! ⚔️          ║\n";
            std::cout <<   "╚══════════════════════════════════════════════╝\n";
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
                Item drop = enemy.generateDrop();
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
            }
            break;
        }
        
        
        
        if (!player.isAlive()) {
            
            std::cout << "\n========================================\n";
            std::cout << "            DEFEAT!                     \n";
            std::cout << "========================================\n";
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
    Enemy finalBoss("Dragon King", 12, EnemyType::DRAGON); // Level 12 boss
    std::cout << R"(
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

)";
std::cout<< "1.Kneel before mighty Dragon King\n";
std::cout<< "2.Remember what he took from you, your dead wife and child, draw your weapon and charge!\n";
std::cout<< "Choice: ";
int choice;
std::cin>> choice;
Beep(200,100);
if(choice == 1) {
    std::cout<< "You kneel before the Dragon King, who smirks and says\n";
    std::cout << R"(
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

)";
    gameOver();
} else {
    std::cout << R"(
🔥 Your hand tightens around your weapon.
   You rise, defiance burning brighter than fear.

🐉 Dragon King: "Ah... so the lamb bares fangs."

His wings spread wide, casting a shadow that swallows the battlefield.

⚔️ You charge — heart pounding like war drums,
   steel singing your final vow.

💥 Flame meets flesh. Roar meets rage.
   Destiny ignites in a clash no bard will ever forget...

        ⚔️ 𝔸 𝕃𝔼𝔾𝔼ℕ𝔻 𝕀𝕊 𝔹𝕆ℝℕ 𝕀ℕ 𝔽𝕀ℝ𝔼 ⚔️

)";
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
        std::cout << "That's why you are a peasant! You can't afford to rest!\n";
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

    Item weapon = Item::generateRandomItem();
    Item armor = Item::generateRandomItem();

    while (inMarket) {
        std::cout << "\nYou find a bustling market!\n";
        std::cout << "Your current gold: " << player.getGold() << "\n";

        std::cout << "\nItems for sale:\n";
        std::cout << "1. " << weapon.getName() << " " << weapon.getStatsString()
                  << " (Cost: " << static_cast<int>(weapon.getValue() / player.getBriberySkill()) << " gold, discount: "
                  << static_cast<int>((player.getBriberySkill() - 1.0f) * 100) << "%)\n";
        std::cout << "2. " << armor.getName() << " " << armor.getStatsString()
                  << " (Cost: " << static_cast<int>(armor.getValue() / player.getBriberySkill()) << " gold, discount: "
                  << static_cast<int>((player.getBriberySkill() - 1.0f) * 100) << "%)\n";
        std::cout << "3. Reroll stocks (Cost: " << rerollCost << " gold)\n";
        std::cout << "4. Leave market\n";
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        Beep(200, 100);

        switch (choice) {
            case 1: {
                int price = weapon.getValue() / player.getBriberySkill();
                if (player.spendGold(price)) {
                    player.equipWeapon(weapon);
                    std::cout << "You purchase and equip the " << weapon.getName() << "!\n";
                } else {
                    std::cout << "You don't have enough gold! (Need " << price << ")\n";
                }
                break;
            }
            case 2: {
                int price = armor.getValue() / player.getBriberySkill();
                if (player.spendGold(price)) {
                    player.equipArmor(armor);
                    std::cout << "You purchase and equip the " << armor.getName() << "!\n";
                } else {
                    std::cout << "You don't have enough gold! (Need " << price << ")\n";
                }
                break;
            }
            case 3:
                if (player.getGold() >= rerollCost) {
                    player.spendGold(rerollCost);
                    weapon = Item::generateRandomItem();
                    armor = Item::generateRandomItem();
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
            // First mini-boss: 50% common, 40% rare, 10% legendary
            relic1 = rollRelic(50-player.getLuck(), 40);
            relic2 = rollRelic(50-player.getLuck(), 40);
            relic3 = rollRelic(50-player.getLuck(), 40);
        } else {
            // Second mini-boss: 60% rare, 25% common, 15% legendary
            relic1 = rollRelic(25-player.getLuck(), 60-player.getLuck());
            relic2 = rollRelic(25-player.getLuck(), 60-player.getLuck());
            relic3 = rollRelic(25-player.getLuck(), 60-player.getLuck());
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
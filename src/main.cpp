#include "../include/Game.h"
#include <iostream>

int main() {
    try {
        std::cout << "Welcome to the Wandering Peasant (ver 1.01)!\n";
        std::cout << "------------------------\n\n";
        std::cout << "You are but a humble peasant, born of soil and sweat, wandering the vast and perilous world.\n";
        std::cout << "For years, your hands tilled the earth, your heart content beneath a thatched roof and golden fields. But fate is cruel. \n";
        std::cout << "One cursed twilight, a shadow blotted the sun—a Dragon, black as obsidian and cruel as winter, descended upon your village.\n";
        std::cout << "With fire and fury, it razed your farm to ashes, leaving only smoke and sorrow.\n";
        std::cout << "Nothing left to lose, you clutch a rusty sword, steel your heart, and step onto the open road!\n\n";
        std::cout << "This is the story of THE WANDERING PEASANT. Let the adventure begin!⚔️\n\n";
        Game game;
        game.run();
        
        std::cout << "\nThanks for playing!\n";
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown error occurred!" << std::endl;
        return 1;
    }
} 
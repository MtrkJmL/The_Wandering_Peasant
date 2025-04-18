#include "../include/NPC.h"


std::string NPC::getBardSongName(BardSong song) const {
    switch (song) {
        case BardSong::BALLAD_OF_KIRIK: return "Ballad of Kirik";
        case BardSong::WHISPERS_OF_THE_WYRM: return "Whispers of the Wyrm";
        case BardSong::ECLIPSEFIRE_LAMENT: return "Eclipsefire Lament";
        case BardSong::MARCH_OF_THE_LOST_CROWNS: return "March of the Lost Crowns";
        case BardSong::DIRGE_OF_THE_DARKBLOODS: return "Dirge of the Darkbloods";
        case BardSong::HYMNS_FROM_THE_HOLLOW: return "Hymns from the Hollow";
        case BardSong::TAVERN_TUNE_OF_GLORY: return "Tavern Tune of Glory";
        case BardSong::ODE_TO_THE_LAST_FLAME: return "Ode to the Last Flame";
        case BardSong::DANCE_OF_THE_STARBOUND: return "Dance of the Starbound";
        case BardSong::CHANT_OF_THE_MOONWATCHERS: return "Chant of the Moonwatchers";
    }
}

std::string NPC::getBardSongVerse(BardSong song) const {
    switch (song) {
        case BardSong::BALLAD_OF_KIRIK: return 
        R"( With blade of flame and eyes alight,
            Kirik rose to end the night.
        One swing, one scream, a tyrant slain—
            And silence fell o’er ash and flame.)";
        case BardSong::WHISPERS_OF_THE_WYRM: return 
        R"( In deepest stone where wyrms do creep,
            They whisper secrets while we sleep.
        Beware the dreams that smell of smoke,
            For dragon tongues speak cursed oaths.)";
        case BardSong::ECLIPSEFIRE_LAMENT: return 
        R"( The sun wept blood, the moon turned black,
            As fire rained and skies did crack.
        None stood fast, none did fight—
            When the King of Flame unleashed his might.)";
        case BardSong::MARCH_OF_THE_LOST_CROWNS: return 
        R"( Once proud heads, now nameless bones,
            Their crowns lie rusted, overthrown.
        Still they march in shadowed plains,
            Bound to thrones by ghostly chains.)";
        case BardSong::DIRGE_OF_THE_DARKBLOODS: return 
        R"( Born of sorrow, cursed by kin,
            They walk the world with wrath within.
        Their blood runs thick, but never pure—
            A tale of pain that must endure.)";
        case BardSong::HYMNS_FROM_THE_HOLLOW: return 
        R"( Hollow winds hum hymns to none,
            For all who sang are dead and gone.
        Yet echo still their sacred plea:
            ‘Let memory not forsake the free.’)";
        case BardSong::TAVERN_TUNE_OF_GLORY: return 
        R"( Raise your mugs and bang the drum,
            Tell the tales of battles won!
        Heroes live in froth and song—
            And drunk or dead, we march along!)";
        case BardSong::ODE_TO_THE_LAST_FLAME: return 
        R"( A single spark in endless shade,
            The fire left by those who stayed.
        Though winds may howl and ashes fall,
            That last brave flame defies them all.)";
        case BardSong::DANCE_OF_THE_STARBOUND: return 
        R"( They waltz on winds beyond the sky,
            Where silver rivers twist and fly.
        The starbound dance with fate unchained—
            No throne, no tomb, just dreams retained.)";
        case BardSong::CHANT_OF_THE_MOONWATCHERS: return 
        R"( When moonlight bleeds and wolves all cry,
            The watchers stand with blades held high.
        Warriors of Zmar, sworn and sealed—
            They guard the truths the stars concealed.)";
    }
}

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
#include "vendor_list.h"
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
using namespace std;



// Market price of each item.
unordered_map<string, double> standard_market_price() {

    unordered_map<string, double> smp;

    smp["Charizard"] = 120;
    smp["Pikachu"] = 18;
    smp["Bulbasaur"] = 8;
    smp["Squirtle"] = 11;
    smp["Jigglypuff"] = 8;
    smp["Eevee"] = 12;
    smp["Meowth"] = 8;
    smp["Rizz"] = 8;

    return smp;
}

// Ratings and # of sales of each vendor
/*unordered_map<string, double, int> vendor_trust() {
}*/

// Each card has a list of available vendors. Each vendor has information of the price + shipping in a pair, along with quantity of the items available.
// When calling for a certain card, it will sort through the map of that item.
unordered_map<string, unordered_map<string, pair<pair<double, double>, int>>> get_vendor_data() {
    unordered_map<std::string, std::unordered_map<std::string, std::pair<std::pair<double, double>, int>>> vendor_data;

    vendor_data["Charizard"] = {
        {"PokeMartA", {{120.0, 5.0}, 10}},
        {"PokeStop99", {{130.0, 2.5}, 5}},
        {"Cards4U", {{115.0, 7.0}, 7}},
        {"EliteTrainers", {{118.0, 4.5}, 3}},
        {"DragonDeck", {{125.0, 0.0}, 8}},
        {"FireForge", {{123.0, 2.0}, 4}},
        {"RarePulls", {{121.0, 3.0}, 6}},
        {"BattleDepot", {{119.0, 2.5}, 2}},
        {"HeatHaven", {{122.0, 1.5}, 1}},
        {"InfernoTraders", {{117.5, 4.0}, 9}},
        {"BurningDecks", {{126.0, 1.0}, 2}},
        {"LegendaryLair", {{124.5, 0.5}, 10}},
        {"PhoenixCards", {{127.0, 2.0}, 1}},
        {"FlameMarket", {{118.5, 3.0}, 7}},
        {"MysticVault", {{116.5, 5.0}, 5}},
        {"DeckRush", {{122.0, 0.0}, 1}},
        {"TopDecks", {{129.0, 1.2}, 3}},
        {"HotZone", {{128.0, 1.8}, 3}},
        {"RareStash", {{119.5, 2.3}, 6}},
        {"FlareTraders", {{121.5, 1.7}, 3}}
    };

    vendor_data["Pikachu"] = {
        {"PokeMartA", {{15.0, 5.0}, 8}},
        {"EliteTrainers", {{18.0, 0.0}, 2}},
        {"Cards4U", {{14.0, 4.0}, 10}},
        {"ThunderBay", {{16.5, 2.0}, 15}},
        {"VoltVault", {{13.0, 6.0}, 3}},
        {"FlameMarket", {{6, 2.0}, 1}},
        {"ZapZone", {{17.5, 1.5}, 4}},
        {"PokeStop99", {{20.0, 3.5}, 7}},
        {"RaichuTraders", {{19.0, 1.2}, 6}},
        {"SparkStore", {{21.0, 2.0}, 5}},
        {"ElectricEmpire", {{22.0, 0.5}, 9}}
    };

    vendor_data["Bulbasaur"] = {
        {"Cards4U", {{7.5, 2.5}, 6}},
        {"GreenLeafTraders", {{6.0, 3.5}, 9}},
        {"NatureDepot", {{6.7, 1.5}, 4}},
        {"PokeMartA", {{8.0, 2.0}, 3}},
        {"LeafyPoke", {{7.8, 4.5}, 5}},
        {"VineVault", {{6.5, 1.0}, 7}},
        {"EcoStore", {{9.0, 2.5}, 6}},
        {"WildRoots", {{7.2, 3.0}, 8}},
        {"PokeDepot", {{7.0, 4.0}, 2}},
        {"BulbaBox", {{8.5, 2.0}, 6}}
    };

    vendor_data["Squirtle"] = {
        {"WaterWorld", {{10.0, 4.0}, 3}},
        {"PokeMartA", {{12.0, 1.5}, 5}},
        {"SplashShop", {{11.0, 3.0}, 8}},
        {"TurtleBay", {{13.0, 2.0}, 6}},
        {"BlueStream", {{14.0, 1.0}, 4}},
        {"AquaTraders", {{9.5, 2.5}, 10}},
        {"SquirtlePond", {{11.5, 3.5}, 7}},
        {"SeaSupply", {{10.5, 4.0}, 3}},
        {"WaterWave", {{12.5, 1.0}, 6}},
        {"TidalTrade", {{13.5, 0.0}, 4}}
    };

    vendor_data["Jigglypuff"] = {
        {"SingStore", {{8.0, 5.0}, 5}},
        {"PokeMartA", {{7.5, 4.0}, 3}},
        {"SleepyPoke", {{8.5, 2.5}, 4}},
        {"DreamShop", {{9.0, 3.0}, 8}},
        {"JigglyJunk", {{7.0, 5.0}, 6}},
        {"MelodyMarket", {{6.5, 2.0}, 7}},
        {"PuffPals", {{9.0, 4.0}, 2}},
        {"LullabyTraders", {{7.5, 6.0}, 9}},
        {"HarmonyStore", {{8.0, 3.5}, 4}},
        {"SleepySing", {{7.8, 4.2}, 6}}
    };

    vendor_data["Eevee"] = {
        {"EeveeShop", {{10.0, 4.5}, 8}},
        {"PokeMartA", {{9.5, 3.5}, 7}},
        {"FurryFriends", {{11.0, 2.0}, 10}},
        {"EeveePals", {{12.0, 1.0}, 9}},
        {"EvolutionaryEdge", {{13.0, 3.0}, 6}},
        {"FurSupply", {{12.5, 4.0}, 5}},
        {"EeveeNest", {{10.5, 2.0}, 4}},
        {"ShinyStore", {{14.0, 3.0}, 3}},
        {"PokeStop99", {{15.0, 1.5}, 2}},
        {"EeveeTales", {{13.5, 5.0}, 8}}
    };

    vendor_data["Meowth"] = {
        {"PokeMartA", {{8.0, 3.0}, 7}},
        {"CatCorner", {{7.5, 4.0}, 10}},
        {"GoldPaw", {{6.0, 5.5}, 3}},
        {"MeowthMarket", {{9.0, 2.0}, 8}},
        {"PawTrade", {{7.5, 1.5}, 6}},
        {"KittyCards", {{8.5, 3.5}, 5}},
        {"MeowStop", {{10.0, 0.0}, 4}},
        {"PurrPoke", {{11.0, 2.5}, 7}},
        {"CatCastle", {{9.5, 1.0}, 6}},
        {"GoldTail", {{12.0, 3.0}, 9}}
    };

    vendor_data["Rizz"] = {
        {"One", {{8.0, 3.0}, 1}},
        {"Two", {{7.5, 4.0}, 2}},

    };

    return vendor_data;
}
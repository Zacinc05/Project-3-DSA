#include "vendor_list.h"
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <random>
#include <algorithm>
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

    // initializing randomization factors
    random_device true_random;
    mt19937 generator(true_random());
    uniform_int_distribution<> quantity_dist(1, 15);
    uniform_real_distribution<> shipping_dist(0.0, 35.0);
    uniform_real_distribution<> card_available_probability(0.0, 1.0);

    // Actual list of vendor data to be returned after function execution
    unordered_map<std::string, std::unordered_map<std::string, std::pair<std::pair<double, double>, int>>> vendor_data;

    // Hardcoded list of Pokémon cards along with their average MSRP for being first-generation and ungraded
    vector<pair<string, double>> pokemon = {
            {"Bulbasaur", 75.00}, {"Ivysaur", 150.00}, {"Venusaur", 300.00},
            {"Charmander", 100.00}, {"Charmeleon", 200.00}, {"Charizard", 3000.00},
            {"Squirtle", 80.00}, {"Wartortle", 175.00}, {"Blastoise", 400.00},
            {"Caterpie", 15.00}, {"Metapod", 20.00}, {"Butterfree", 30.00},
            {"Weedle", 12.00}, {"Kakuna", 18.00}, {"Beedrill", 25.00},
            {"Pidgey", 20.00}, {"Pidgeotto", 30.00}, {"Pidgeot", 50.00},
            {"Rattata", 10.00}, {"Raticate", 15.00}, {"Spearow", 12.00},
            {"Fearow", 25.00}, {"Ekans", 15.00}, {"Arbok", 25.00},
            {"Pikachu", 300.00}, {"Raichu", 40.00}, {"Sandshrew", 20.00},
            {"Sandslash", 35.00}, {"Nidoran♀", 20.00}, {"Nidorina", 30.00},
            {"Nidoqueen", 60.00}, {"Nidoran♂", 20.00}, {"Nidorino", 30.00},
            {"Nidoking", 60.00}, {"Clefairy", 30.00}, {"Clefable", 50.00},
            {"Vulpix", 40.00}, {"Ninetales", 75.00}, {"Jigglypuff", 25.00},
            {"Wigglytuff", 35.00}, {"Zubat", 10.00}, {"Golbat", 20.00},
            {"Oddish", 20.00}, {"Gloom", 30.00}, {"Vileplume", 40.00},
            {"Paras", 12.00}, {"Parasect", 18.00}, {"Venonat", 15.00},
            {"Venomoth", 25.00}, {"Diglett", 15.00}, {"Dugtrio", 25.00},
            {"Meowth", 30.00}, {"Persian", 40.00}, {"Psyduck", 20.00},
            {"Golduck", 30.00}, {"Mankey", 12.00}, {"Primeape", 25.00},
            {"Growlithe", 50.00}, {"Arcanine", 120.00}, {"Poliwag", 20.00},
            {"Poliwhirl", 30.00}, {"Poliwrath", 40.00}, {"Abra", 25.00},
            {"Kadabra", 35.00}, {"Alakazam", 70.00}, {"Machop", 15.00},
            {"Machoke", 25.00}, {"Machamp", 40.00}, {"Bellsprout", 15.00},
            {"Weepinbell", 20.00}, {"Victreebel", 30.00}, {"Tentacool", 10.00},
            {"Tentacruel", 20.00}, {"Geodude", 15.00}, {"Graveler", 25.00},
            {"Golem", 35.00}, {"Ponyta", 30.00}, {"Rapidash", 45.00},
            {"Slowpoke", 20.00}, {"Slowbro", 30.00}, {"Magnemite", 25.00},
            {"Magneton", 40.00}, {"Farfetch'd", 30.00}, {"Doduo", 15.00},
            {"Dodrio", 25.00}, {"Seel", 15.00}, {"Dewgong", 25.00},
            {"Grimer", 12.00}, {"Muk", 20.00}, {"Shellder", 10.00},
            {"Cloyster", 30.00}, {"Gastly", 20.00}, {"Haunter", 30.00},
            {"Gengar", 250.00},
            {"Onix", 35.00}, {"Drowzee", 18.00}, {"Hypno", 28.00},
            {"Krabby", 10.00}, {"Kingler", 20.00}, {"Voltorb", 15.00},
            {"Electrode", 25.00}, {"Exeggcute", 20.00}, {"Exeggutor", 30.00},
            {"Cubone", 25.00}, {"Marowak", 35.00}, {"Hitmonlee", 40.00},
            {"Hitmonchan", 40.00}, {"Lickitung", 30.00}, {"Koffing", 15.00},
            {"Weezing", 25.00}, {"Rhyhorn", 30.00}, {"Rhydon", 45.00},
            {"Chansey", 100.00},
            {"Tangela", 20.00}, {"Kangaskhan", 40.00}, {"Horsea", 10.00},
            {"Seadra", 20.00}, {"Goldeen", 10.00}, {"Seaking", 15.00},
            {"Staryu", 20.00}, {"Starmie", 35.00}, {"Mr. Mime", 30.00},
            {"Scyther", 60.00},
            {"Jynx", 20.00}, {"Electabuzz", 40.00}, {"Magmar", 35.00},
            {"Pinsir", 25.00}, {"Tauros", 30.00}, {"Magikarp", 5.00},
            {"Gyarados", 150.00},
            {"Lapras", 80.00},
            {"Ditto", 25.00}, {"Eevee", 100.00},
            {"Vaporeon", 30.00}, {"Jolteon", 30.00}, {"Flareon", 30.00},
            {"Porygon", 30.00}, {"Omanyte", 20.00}, {"Omastar", 30.00},
            {"Kabuto", 20.00}, {"Kabutops", 30.00}, {"Aerodactyl", 50.00},
            {"Snorlax", 50.00}, {"Articuno", 90.00}, {"Zapdos", 90.00},
            {"Moltres", 90.00}, {"Dratini", 30.00}, {"Dragonair", 60.00},
            {"Dragonite", 250.00},
            {"Mewtwo", 300.00},
            {"Mew", 200.00}
            };

    vector<string> vendor_names = {
            "Card Haven", "The Pokémon Emporium", "Elite Card Traders", "Poké Treasures", "Gotta Catch 'Em All Cards",
            "Trainer's Trove", "Rare Finds Collectibles", "Card Masters", "The Trading Post", "Pokémon Card Paradise",
            "Victory Vault", "Battle Ready Cards", "Card Kingdom", "The Collector's Den", "Poké Mart Plus",
            "Legendary Loot", "Card Quest", "The Card Corner", "Pokémon Card Central", "Trainer's Choice",
            "Ultra Rare Cards", "Card Sharks", "The Card Cave", "Pokémon Card Galaxy", "Champion Cards",
            "Card Craze", "The Card Shop", "Pokémon Card World", "Trainer's Deck", "Epic Card Pulls",
            "Card Mania", "The Card Hub", "Pokémon Card Universe", "Ace Card Traders", "Card Collect",
            "The Card Spot", "Pokémon Card Store", "Trainer's Vault", "Amazing Card Finds", "Card Zone",
            "The Card Place", "Pokémon Card Depot", "Best Card Deals", "Card Source", "The Card Stop",
            "Pokémon Card Center", "Trainer's Market", "Awesome Card Buys", "Card Gallery"};

    // MSRP multipliers - These contain duplicates for the more common MSRP multipliers, and fewer of the less-common ones
    // Aka, more MSRP multiplies in the 0.8-0.9 range for realism
    vector<double> msrp_multipliers = {
            0.70, 0.72, 0.75, 0.78, 0.80, 0.81, 0.82, 0.83, 0.84, 0.85, 0.86, 0.87, 0.88, 0.89, 0.90, 0.91, 0.92, 0.93,
            0.85, 0.87, 0.88, 0.89, 0.90, 0.88, 0.87, 0.85, 0.86, 0.87, 0.88, 0.89, 0.85, 0.86,  0.95, 0.98, 1.00, 1.05,
            1.10, 1.15, 1.20
    };

    // Map used to store vendor shipping costs
    unordered_map<string, double> vendor_shipping_costs;
    for (const auto& vendor : vendor_names)
        vendor_shipping_costs[vendor] = round(shipping_dist(generator) * 100 / 100);

    // Uniform Int Distribution for determining which MSRP multiplier to apply
    uniform_int_distribution<> msrp_multiplier_dist(0, msrp_multipliers.size() - 1);

    // Creating the actual vendor data
    for (const auto& card_info : pokemon){
        // Pulling the card's info through the Pokémon map
        const string& card_name = card_info.first;
        double msrp = card_info.second;

        uniform_int_distribution<> vendor_count_dist(1, vendor_names.size());
        int potential_vendors = vendor_count_dist(generator);

        shuffle(vendor_names.begin(), vendor_names.end(), generator);

        // Determining whether a vendor has a card or not for the Pokémon in question
        for (int i = 0; i < potential_vendors; i++){
            const string& vendor = vendor_names[i];

            // 70% chance for a vendor having a card available
            if (card_available_probability(generator) < 0.7){
                double msrp_mult = msrp_multipliers[msrp_multiplier_dist(generator)];
                double price = round(msrp * msrp_mult * 100) / 100;
                double shipping = vendor_shipping_costs[vendor];
                int quantity = quantity_dist(generator);

                vendor_data[card_name][vendor] = {{price, shipping}, quantity};

            }
        }
    }

    return vendor_data;
}
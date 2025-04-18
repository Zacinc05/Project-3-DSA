#include "vendor_list.h"
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
using namespace std;

// although the item definitely should be there, search it up, and decrement count in shop. if now count = 0, delete from available list
bool remove_from_available(tuple<string, double, int> rem, string item, unordered_map<string, unordered_map<string, pair<pair<double, double>, int>>>& vendor_price_shipping) {
    auto search = vendor_price_shipping.find(item);
    if (search != vendor_price_shipping.end()) {
        auto store = search->second.find(get<0>(rem));
        if (store != search->second.end()) {
            store->second.second--;
            if (store->second.second <= 0) {
                search->second.erase(get<0>(rem));
            }
            return true;
        }
    }
    return false;
}

// find the current cheapest item (+ shipping if not on list)
// will also return the count of how man at same price exist, as it'll be the cheapest item next time as well
tuple<string, double, int> best_fit_cheapest_of_item(unordered_map<string, pair<pair<double, double>, int>>& locations, unordered_map<string, double>& in_cart) {

    // if none available, end. otherwise, search for best current option based on this item's current price (+shipping if necessary)

    if(locations.empty()) {return {"none", -1, -1};}
    cout << "searching between " << locations.size() << " different vendors."<< endl << endl;

    // add the first item in the map as the current 'min' price

    auto itr = locations.begin();
    double lowest;
    lowest = itr->second.first.first;

    string cheapest = itr->first;
    int counter = itr->second.second;

    double shipping;
    if(in_cart.find(cheapest) == in_cart.end()) {shipping = itr->second.first.second;}
    else {shipping = 0;}

    // iterate through the rest to find the best

    itr++;
    while(itr != locations.end())
    {
        double cost = itr->second.first.first;
        double this_shipping = 0;
        if (in_cart.find(itr->first) == in_cart.end()) { this_shipping = itr->second.first.second; }

        if ((cost + this_shipping) < (lowest + shipping)) { lowest = cost; cheapest = itr->first; counter = itr->second.second; shipping = itr->second.first.second;}
        itr++;
    }


    // update the vendors, lowest shipping routes, and return vals of the best item

    cout << "adding from store: " << cheapest << endl;

    if (in_cart.find(cheapest) != in_cart.end())
    {
        if (in_cart[cheapest] > shipping)
        {
            cout << "cheaper shipping option with this seller found: new shipping cost: " << shipping << endl;
            in_cart[cheapest] = shipping;
        } else if (in_cart[cheapest] == shipping) {
            cout << "shipping of this item is same as the same as shipping cost of other item by vendor. no changes in ship price: " << shipping << endl;
        } else {cout << "shipping of this item is more as the same as shipping cost of other item by vendor. no changes in ship price: " << shipping << endl;}
    } else {
        cout << "item from new store. added shipping cost: " << fixed << setprecision(2) << shipping << endl;
        in_cart[cheapest] = shipping;
    }

    return {cheapest, lowest, counter};
}

double best_fit_algorithm(unordered_map<string, unordered_map<string, pair<pair<double, double>, int>>> vendor_price_shipping, priority_queue<tuple<double, int, string>> additions, vector<pair<string,int>>& unavailable) {
    // get the info of the times person wants and how many

    int amount_of;
    string item_name;
    double total;
    unordered_map<string, double> in_cart;

    while (!additions.empty()) {
        item_name = get<2>(additions.top());
        amount_of = get<1>(additions.top());
        additions.pop();


        cout << "Searching for: " << item_name << endl << endl;

        // search for current cheapest option of item

        int i = 0;
        while (i < amount_of) {
            unordered_map<string, pair<pair<double, double>, int>> available_locations = vendor_price_shipping[item_name];

            tuple<string, double, int> cheapest_info = best_fit_cheapest_of_item(available_locations, in_cart);

            int max_iterations = min(get<2>(cheapest_info), amount_of-i);

            if (get<2>(cheapest_info) != -1)
            {
                // decrement it however many times (remaining iterations of card request or number of cards vendor has)

                for(int j = 0; j < max_iterations; j++)
                {
                    if (!remove_from_available(cheapest_info, item_name, vendor_price_shipping))
                    {
                        break;
                    }
                    cout << get<0>(cheapest_info) << "  " << get<1>(cheapest_info) << endl;

                    total += get<1>(cheapest_info);
                    i++;
                }
                cout << endl;
            }
            else {
                cout << "out of this item" << endl << endl;
                unavailable.push_back({item_name, amount_of - i});
                break;
            }
        }
    }

    for (auto itr = in_cart.begin(); itr != in_cart.end(); itr++)
    {
        total += itr->second;
    }

    return total;
}

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
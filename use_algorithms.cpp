#include "use_algorithms.h"
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

double first_fit_algorithm(unordered_map<string, unordered_map<string, pair<pair<double, double>, int>>> vendor_price_shipping, priority_queue<tuple<double, int, string>> additions, vector<pair<string,int>>& unavailable) {
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
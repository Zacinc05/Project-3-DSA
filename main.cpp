#include "vendor_list.h"
#include <iostream>
#include <iomanip>
#include <queue>
#include <tuple>
using namespace std;


int main()
{
    string item, quantity;
    priority_queue<tuple<double, int, string>> item_addition_order;
    unordered_map<string, double> listings = standard_market_price();
    vector<pair<string,int>> unavailable_items;

    while (true) {
        cout << "Enter your item choice:" << endl;
        cin >> item;
        if (item == "-1") {break;}
        cout << "Enter quantity:" << endl;
        cin >> quantity;
        if (quantity == "-1") {break;}

        if(listings.find(item) != listings.end()) {item_addition_order.push({stoi(quantity) * listings[item], stoi(quantity), item});}
        else {
            cout << endl << "Item not found. Please Reenter:" << endl << endl;
        }
    }



    // load copy of the vendor data, create empty list of vendors in my cart. value will be the lowest shipping cost from that vendor.

    double best_fit_sum = best_fit_algorithm(get_vendor_data(), item_addition_order, unavailable_items);

    if(!unavailable_items.empty()) {
        cout << "There are " << unavailable_items.size() << " items that dont have enough in stock" << endl << endl;

        for (auto item : unavailable_items) {
            cout << item.first << ": " << item.second << " missing." << endl;
        }
        cout << endl;
    } else {
        cout << "All items were added to stock." << endl;
    }
    cout << "Your Total:" << endl << "$";
    cout << fixed << setprecision(2) << best_fit_sum << endl;

    return 0;
}
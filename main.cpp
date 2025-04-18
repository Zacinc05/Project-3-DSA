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

    double best_fit_sum = best_fit_algorithm(getVendorData(), item_addition_order);

    cout << fixed << setprecision(2) << best_fit_sum << endl;

    return 0;
}
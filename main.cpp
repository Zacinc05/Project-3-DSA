#include "vendor_list.h"
#include "use_algorithms.h"
#include <iostream>
#include <iomanip>
#include <queue>
#include <tuple>
#include <ctime>
using namespace std;


int main()
{
    string add_item, quantity;
    priority_queue<tuple<double, int, string>> item_addition_order;
    unordered_map<string, double> listings = standard_market_price();
    vector<pair<string,int>> first_fit_unavailable_items;
    vector<pair<string,int>> best_fit_unavailable_items;

    while (true) {
        cout << "Enter your item choice:" << endl;
        cin >> add_item;
        if (add_item == "-1") {break;}
        cout << "Enter quantity:" << endl;
        cin >> quantity;
        if (quantity == "-1") {break;}

        if(listings.find(add_item) != listings.end()) {item_addition_order.push({stoi(quantity) * listings[add_item], stoi(quantity), add_item});}
        else {
            cout << endl << "Item not found. Please Reenter:" << endl << endl;
        }
    }



    // load copy of the vendor data, create empty list of vendors in my cart. value will be the lowest shipping cost from that vendor.
    clock_t b_start = clock();
    double best_fit_sum = best_fit_algorithm(get_vendor_data(), item_addition_order, best_fit_unavailable_items);
    clock_t b_end = clock();
    double b_time = static_cast<double>(b_end - b_start) / CLOCKS_PER_SEC;

    cout << endl << endl << "--------------------------------------------------------------------------------------" << endl << endl << endl;

    clock_t f_start = clock();
    double first_fit_sum = first_fit_algorithm(get_vendor_data(), item_addition_order, first_fit_unavailable_items, listings);
    clock_t f_end = clock();
    double f_time = static_cast<double>(f_end - f_start) / CLOCKS_PER_SEC;

    if(!best_fit_unavailable_items.empty()) {
        cout << "There are " << best_fit_unavailable_items.size() << " items that dont have enough in stock" << endl << endl;

        for (auto item : best_fit_unavailable_items) {
            cout << item.first << ": " << item.second << " missing." << endl;
        }
        cout << endl;
    } else {
        cout << "All items were added to stock." << endl;
    }
    cout << "Your Total:" << endl << "$";
    cout << fixed << setprecision(2) << best_fit_sum << endl;
    cout << setprecision(5) << "Time to calculate: " << b_time << " seconds." << endl;

    if(!first_fit_unavailable_items.empty()) {
        cout << "There are " << first_fit_unavailable_items.size() << " items that dont have enough in stock" << endl << endl;

        for (auto item : first_fit_unavailable_items) {
            cout << item.first << ": " << item.second << " missing." << endl;
        }
        cout << endl;
    } else {
        cout << "All items were added to stock." << endl;
    }

    cout << "Your Total:" << endl << "$";
    cout << fixed << setprecision(2) << first_fit_sum << endl;
    cout << setprecision(5) << "Time to calculate: " << f_time << " seconds." << endl;

    return 0;
}
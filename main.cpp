#include "vendor_list.h"
#include "use_algorithms.h"
#include "Applicationwindow.h"
#include <iostream>
#include <iomanip>
#include <queue>
#include <tuple>
#include <ctime>
#include <regex>
#include <sstream>
using namespace std;

int main()
{
    string add_item, quantity;
    int star_rating, number_of_sales;
    priority_queue<tuple<double, int, string>> item_addition_order;
    unordered_map<string, double> listings = standard_market_price();
    vector<pair<string,int>> first_fit_unavailable_items;
    vector<pair<string,int>> best_fit_unavailable_items;


    // Gets user input for star rating and sales specification, only loops because of input validation
    cout << "Enter your preference on minimum vendor star ratings (1 being worst and 5 being best):" << endl;
    while (true) {
        string input;
        cin >> input;
        if (regex_match(input, regex("[0-9]"))) {
            if (stoi(input) >= 1 && stoi(input) <= 5) {
                star_rating = stoi(input);
                break;
            }
            cout << "Please enter a number between 1 and 5" << endl;
        } else {
            cout << "Please enter a valid input." << endl;
        }
    }

    cout << "Enter your preference on minimum number of sales a vendor needs to have made (1 for 10,000+ sales, 2 for 20,000+ sales, etc):" << endl;
    while (true) {
        string input;
        cin >> input;
        if (regex_match(input, regex("\\d+"))) {
            number_of_sales = stoi(input);
            break;
        }
        cout << "Please enter a valid input." << endl;
    }

    // Creates info on star ratings and # of sales for each vendor
    unordered_map<string, pair<double, int>> reviews = vendor_trust();

    // Creates the vendor data and also creates a filtered list of vendors
    unordered_map<string, unordered_map<string, pair<pair<double, double>, int>>> vendor_data = get_vendor_data();
    unordered_map<string, unordered_map<string, pair<pair<double, double>, int>>> filtered_data = filtered_vendors(vendor_data, reviews, star_rating, number_of_sales);

    // Loops to get all user input on card choices and quantity specification
    while (true) {
        cout << "Enter your item choice (-1 if finished):" << endl;
        cin >> add_item;
        if (add_item == "-1") {break;}
        if(listings.find(add_item) == listings.end()) {
            cout << endl << "Item not found. Please Reenter:" << endl << endl;
            continue;
        }

        string vendor_listings = vendor_card_info(vendor_data[add_item], reviews);

        cout << "Enter quantity (-1 if finished):" << endl;
        cin >> quantity;
        if (quantity == "-1") {break;}

        if (!regex_match(quantity, regex("[1-99]"))) {
            cout << "Please enter a valid input." << endl;
            continue;
        }
        item_addition_order.push({stoi(quantity) * listings[add_item], stoi(quantity), add_item});
    }

    // load copy of the vendor data, create empty list of vendors in my cart. value will be the lowest shipping cost from that vendor.
    clock_t b_start = clock();
    pair<double,int> best_fit_sum = best_fit_algorithm(filtered_data, item_addition_order, best_fit_unavailable_items, reviews);
    clock_t b_end = clock();
    double b_time = static_cast<double>(b_end - b_start) * 1000 / CLOCKS_PER_SEC;

    cout << endl << endl << "--------------------------------------------------------------------------------------" << endl << endl << endl;

    clock_t f_start = clock();
    pair<double,int> first_fit_sum = first_fit_algorithm(filtered_data, item_addition_order, first_fit_unavailable_items, listings, reviews);
    clock_t f_end = clock();
    double f_time = static_cast<double>(f_end - f_start) * 1000 / CLOCKS_PER_SEC;

    cout << endl << endl << "--------------------------------------------------------------------------------------" << endl << endl << endl;

    ostringstream oss;

    if (!best_fit_unavailable_items.empty()) {
        oss << "There are " << best_fit_unavailable_items.size() << " items that don't have enough in stock\n\n";
        for (const auto& item : best_fit_unavailable_items) {
            oss << item.first << ": " << item.second << " missing.\n";
        }
        oss << "\n";
    } else {
        oss << "All items were added to stock.\n";
    }

    oss << "Your Total for Best Fit:\n$";
    oss << fixed << setprecision(2) << best_fit_sum.first << "\n";
    oss << "Number of vendors to ship items: " << best_fit_sum.second << "\n";
    oss << setprecision(5) << "Time to calculate: " << b_time << " milliseconds.\n\n";

    if (!first_fit_unavailable_items.empty()) {
        oss << "There are " << first_fit_unavailable_items.size() << " items that don't have enough in stock\n\n";
        for (const auto& item : first_fit_unavailable_items) {
            oss << item.first << ": " << item.second << " missing.\n";
        }
        oss << "\n";
    } else {
        oss << "All items were added to stock.\n";
    }

    oss << "Your Total for Best Fit:\n$";
    oss << fixed << setprecision(2) << first_fit_sum.first << "\n";
    oss << "Number of vendors to ship items: " << first_fit_sum.second << "\n";
    oss << setprecision(5) << "Time to calculate: " << f_time << " milliseconds.\n";

    string output_text = oss.str();
    cout << output_text << endl;

    return 0;
}
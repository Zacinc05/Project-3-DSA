// vendor_list.h
#ifndef VENDOR_LIST_H
#define VENDOR_LIST_H

#include <unordered_map>
#include <queue>
#include <string>
#include <utility>
using namespace std;

unordered_map<std::string, std::unordered_map<std::string, std::pair<std::pair<double, double>, int>>> get_vendor_data();

// Will make a way so only include from a vendor based on a certain rating.
//unordered_map<string, double, int> vendor_trust();

unordered_map<string, double> standard_market_price();

bool remove_from_available(tuple<string, double, int> rem, string item, unordered_map<string, unordered_map<string, pair<pair<double, double>, int>>>& vendor_price_shipping);

tuple<string, double, int> cheapest_of_item(unordered_map<string, pair<pair<double, double>, int>>& locations, unordered_map<string, double>& in_cart);

double best_fit_algorithm(unordered_map<string, unordered_map<string, pair<pair<double, double>, int>>> vendor_price_shipping, priority_queue<tuple<double, int, string>> additions, vector<pair<string,int>>& unavailable);

#endif
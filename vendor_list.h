// vendor_list.h
#ifndef VENDOR_LIST_H
#define VENDOR_LIST_H

#include <unordered_map>
#include <string>
#include <utility>
using namespace std;

unordered_map<std::string, std::unordered_map<std::string, std::pair<std::pair<double, double>, int>>> getVendorData();

bool remove_from_available(tuple<string, double, int> rem, string item, unordered_map<string, unordered_map<string, pair<pair<double, double>, int>>>& vendor_price_shipping);

tuple<string, double, int> cheapest_of_item(unordered_map<string, pair<pair<double, double>, int>>& locations, unordered_map<string, double>& in_cart);

double best_fit_algorithm(unordered_map<string, unordered_map<string, pair<pair<double, double>, int>>> vendor_price_shipping);

#endif
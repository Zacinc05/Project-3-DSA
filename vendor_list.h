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

#endif
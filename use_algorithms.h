#include <unordered_map>
#include <queue>
#include <string>
#include <utility>
using namespace std;

#ifndef USE_ALGORITHMS_H
#define USE_ALGORITHMS_H

bool remove_from_available(tuple<string, double, int> rem, string item, unordered_map<string, unordered_map<string, pair<pair<double, double>, int>>>& vendor_price_shipping);

tuple<string, double, int> cheapest_of_item(unordered_map<string, pair<pair<double, double>, int>>& locations, unordered_map<string, double>& in_cart);

double best_fit_algorithm(unordered_map<string, unordered_map<string, pair<pair<double, double>, int>>> vendor_price_shipping, priority_queue<tuple<double, int, string>> additions, vector<pair<string,int>>& unavailable);


#endif //USE_ALGORITHMS_H

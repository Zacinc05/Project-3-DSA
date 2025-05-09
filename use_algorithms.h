#include <unordered_map>
#include <queue>
#include <string>
#include <utility>
using namespace std;

#ifndef USE_ALGORITHMS_H
#define USE_ALGORITHMS_H

bool remove_from_available(tuple<string, double, int> rem, string item, unordered_map<string, unordered_map<string, pair<pair<double, double>, int>>>& vendor_price_shipping);

tuple<string, double, int> best_fit_cheapest_of_item(unordered_map<string, pair<pair<double, double>, int>>& locations, unordered_map<string, double>& in_cart, unordered_map<string, pair<double, int>> reviews);

pair<double,int> best_fit_algorithm(unordered_map<string, unordered_map<string, pair<pair<double, double>, int>>> vendor_price_shipping, priority_queue<tuple<double, int, string>> additions, vector<pair<string,int>>& unavailable, unordered_map<string, pair<double, int>> reviews);

tuple<string, double, int> first_fit_cheapest_of_item(unordered_map<string, pair<pair<double, double>, int>>& locations, unordered_map<string, double>& in_cart, double ovr_market_price, unordered_map<string, pair<double, int>> reviews);

pair<double,int> first_fit_algorithm(unordered_map<string, unordered_map<string, pair<pair<double, double>, int>>> vendor_price_shipping, priority_queue<tuple<double, int, string>> additions, vector<pair<string,int>>& unavailable, unordered_map<string, double> market_price, unordered_map<string, pair<double, int>> reviews);

tuple<string, double, int> first_fit_add_from_cart_vendor(unordered_map<string, pair<pair<double, double>, int>>& locations, unordered_map<string, double>& in_cart, double ovr_market_price, unordered_map<string, pair<double, int>> reviews);

string vendor_card_info(unordered_map<string, pair<pair<double, double>, int>> data, unordered_map<string, pair<double, int>> reviews);


#endif //USE_ALGORITHMS_H

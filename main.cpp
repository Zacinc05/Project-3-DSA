#include "vendor_list.h"
#include <iostream>
#include <iomanip>
#include <queue>
using namespace std;


int main()
{
    string item, quantity;

    while (true) {
        cin >> item;
        if (item == "-1") {break;}
        cin >> quantity;
        if (quantity == "-1") {break;}
    }



    // load copy of the vendor data, create empty list of vendors in my cart. value will be the lowest shipping cost from that vendor.

    double best_fit_sum = best_fit_algorithm(getVendorData());

    cout << fixed << setprecision(2) << best_fit_sum << endl;

    return 0;
}
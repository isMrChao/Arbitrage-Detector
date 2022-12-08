#include <iostream>
#include "arbitrage.h"

int main() {
    Arbitrage arbitrage("exchange_rate.csv", "location.csv");

    vector<vector<string>> result = arbitrage.GetArbitrage();
    for (auto & arbitrage_path : result) {
        double conversion_rate = 1;
        for (size_t i = 0; i < arbitrage_path.size(); i++) {
            if (i == arbitrage_path.size() - 1) {
                std::cout << arbitrage_path[i] << " at rate: "
                // print the conversion rate in full
                << std::fixed << std::setprecision(8) << conversion_rate << std::endl;
            } else {
                std::cout << arbitrage_path[i] << " -> ";
                conversion_rate *= arbitrage.GetExchangeRate(arbitrage_path[i], arbitrage_path[i + 1]);
            }
        }
    }
    cout << endl;
    cout << "Direct conversion from HKD to ILS: " << arbitrage.GetExchangeRate("HKD", "ILS") << endl;
    cout << "Better exchange rate from HKD to ILS: ";
    vector<string> exchange_strategy = arbitrage.GetBetterExchangeRate("HKD", "ILS");
    for (size_t i = 0; i < exchange_strategy.size(); i++) {
        if (i == exchange_strategy.size() - 1) {
            std::cout << "; final exchange rate: "<< exchange_strategy[i] << std::endl;
        } else if (i == exchange_strategy.size() - 2) {
            std::cout << exchange_strategy[i];
        } else {
            std::cout << exchange_strategy[i] << " -> ";
        }
    }
    return 0;
}

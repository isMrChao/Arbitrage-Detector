#include <iostream>
#include "arbitrage.h"

int main() {
    Arbitrage arbitrage("exchange_rate.csv", "location.csv");
    if (arbitrage.IsArbitrage()) {
        std::cout << "Arbitrage found: ";
        vector<string> arbitrage_path = arbitrage.GetArbitrage();
        for (size_t i = 0; i < arbitrage_path.size(); i++) {
            if (i == arbitrage_path.size() - 1) {
                std::cout << arbitrage_path[i] << std::endl;
            } else {
                std::cout << arbitrage_path[i] << " -> ";
            }
        }
    } else {
        cout << "Arbitrage does not exist!" << endl;
    }
    cout << "TRY to IDR: " << arbitrage.GetExchangeRate("TRY", "IDR") << endl;
    cout << "IDR to TRY: " << arbitrage.GetExchangeRate("IDR", "TRY") << endl;
    cout << "USD to CNY: " << arbitrage.GetExchangeRate("USD", "CNY") << endl;
    cout << "CNY to USD: " << arbitrage.GetExchangeRate("CNY", "USD") << endl;
    cout << "CNY to CNY: " << arbitrage.GetExchangeRate("CNY", "CNY") << endl;
    cout << "XXX to CNY: " << arbitrage.GetExchangeRate("XXX", "CNY") << endl;
    return 0;
}

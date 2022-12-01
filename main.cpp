#include <iostream>
#include "arbitrage.h"

int main() {
    Arbitrage arbitrage("exchange_rate.csv", "location.csv");
    cout << arbitrage.IsArbitrage() << endl;
    cout << "USD to CNY: " << arbitrage.GetExchangeRate("USD", "CNY") << endl;
    cout << "CNY to USD: " << arbitrage.GetExchangeRate("CNY", "USD") << endl;
    cout << "CNY to CNY: " << arbitrage.GetExchangeRate("CNY", "CNY") << endl;
    cout << "XXX to CNY: " << arbitrage.GetExchangeRate("XXX", "CNY") << endl;
    return 0;
}

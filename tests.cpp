#include "catch.hpp"
#include "arbitrage.h"

using namespace std;

bool isPresent(const vector<string>& v, const string& s) {
    for (auto& i : v) {
        if (i == s) return true;
    }
    return false;
}

bool hasCycleHelper(const vector<string>& path, const vector<string>& cycle) {
    if ((path.size() - 1) != cycle.size()) return false;
    // align the path and cycle
    size_t start = 0;
    bool flag = false;
    for (size_t i = 0; i < path.size() - 1; i++) {
        if (path[i] == cycle[0]) {
            start = i;
            flag = true;
            break;
        }
    }
    if (!flag) return false;
    for (size_t i = 0; i < cycle.size() - 1; i++) {
        if (path[(start + i) % (path.size() - 1)] != cycle[i]) return false;
    }
    return true;
}

bool hasCycle(const vector<vector<string>>& paths, const vector<string>& cycle) {
    for (const auto& path : paths) {
        if (hasCycleHelper(path, cycle)) return true;
    }
    return false;
}

TEST_CASE("Test GetExchangeRate", "[get_exchange_rate]") {
    Arbitrage arbitrage("exchange_rate_test.csv", "location_test.csv");
    SECTION("Random Basic Exchanges") {
        REQUIRE(arbitrage.GetExchangeRate("USD", "EUR") == 0.741);
        REQUIRE(arbitrage.GetExchangeRate("EUR", "USD") == 1.349);
        REQUIRE(arbitrage.GetExchangeRate("GBP", "CAD") == 1.538);
        REQUIRE(arbitrage.GetExchangeRate("CAD", "GBP") == 0.650);
    } SECTION("Same Currency") {
        REQUIRE(arbitrage.GetExchangeRate("USD", "USD") == 1);
        REQUIRE(arbitrage.GetExchangeRate("EUR", "EUR") == 1);
        REQUIRE(arbitrage.GetExchangeRate("GBP", "GBP") == 1);
        REQUIRE(arbitrage.GetExchangeRate("CHF", "CHF") == 1);
        REQUIRE(arbitrage.GetExchangeRate("CAD", "CAD") == 1);
    } SECTION("Invalid Currency") {
        REQUIRE(arbitrage.GetExchangeRate("USD", "CNY") == 0);
        REQUIRE(arbitrage.GetExchangeRate("CNY", "USD") == 0);
        REQUIRE(arbitrage.GetExchangeRate("CNY", "CNY") == 0);
        REQUIRE(arbitrage.GetExchangeRate("EUR", "") == 0);
        REQUIRE(arbitrage.GetExchangeRate("", "EUR") == 0);
        REQUIRE(arbitrage.GetExchangeRate("", "") == 0);
    }
}

TEST_CASE("Test GetCurrencyList", "[get_currency_list]") {
    Arbitrage arbitrage("exchange_rate_test.csv", "location_test.csv");
    SECTION("North America") {
        vector<string> NA = arbitrage.GetCurrencyList("North America");
        REQUIRE(NA.size() == 2);
        REQUIRE(isPresent(NA, "USD"));
        REQUIRE(isPresent(NA, "CAD"));
    } SECTION("Europe") {
        vector<string> EU = arbitrage.GetCurrencyList("Europe");
        REQUIRE(EU.size() == 3);
        REQUIRE(isPresent(EU, "EUR"));
        REQUIRE(isPresent(EU, "GBP"));
        REQUIRE(isPresent(EU, "CHF"));
    } SECTION("Asia") {
        vector<string> AS = arbitrage.GetCurrencyList("Asia");
        REQUIRE(AS.empty());
    } SECTION("Invalid Region") {
        vector<string> invalid = arbitrage.GetCurrencyList("Invalid");
        REQUIRE(invalid.empty());
    }
}

TEST_CASE("Test Arbitrage Constructor", "[constructor]") {
    Arbitrage arbitrage("exchange_rate_test.csv", "location_test.csv");
    // check if the adjacency matrix is constructed correctly
    SECTION("USD - XXX") {
        REQUIRE(arbitrage.GetExchangeRate("USD", "USD") == 1);
        REQUIRE(arbitrage.GetExchangeRate("USD", "EUR") == 0.741);
        REQUIRE(arbitrage.GetExchangeRate("USD", "GBP") == 0.657);
        REQUIRE(arbitrage.GetExchangeRate("USD", "CHF") == 1.061);
        REQUIRE(arbitrage.GetExchangeRate("USD", "CAD") == 1.005);
    } SECTION("EUR - XXX") {
        REQUIRE(arbitrage.GetExchangeRate("EUR", "USD") == 1.349);
        REQUIRE(arbitrage.GetExchangeRate("EUR", "EUR") == 1);
        REQUIRE(arbitrage.GetExchangeRate("EUR", "GBP") == 0.888);
        REQUIRE(arbitrage.GetExchangeRate("EUR", "CHF") == 1.433);
        REQUIRE(arbitrage.GetExchangeRate("EUR", "CAD") == 1.366);
    } SECTION("GBP - XXX") {
        REQUIRE(arbitrage.GetExchangeRate("GBP", "USD") == 1.521);
        REQUIRE(arbitrage.GetExchangeRate("GBP", "EUR") == 1.126);
        REQUIRE(arbitrage.GetExchangeRate("GBP", "GBP") == 1);
        REQUIRE(arbitrage.GetExchangeRate("GBP", "CHF") == 1.614);
        REQUIRE(arbitrage.GetExchangeRate("GBP", "CAD") == 1.538);
    } SECTION("CHF - XXX") {
        REQUIRE(arbitrage.GetExchangeRate("CHF", "USD") == 0.942);
        REQUIRE(arbitrage.GetExchangeRate("CHF", "EUR") == 0.698);
        REQUIRE(arbitrage.GetExchangeRate("CHF", "GBP") == 0.619);
        REQUIRE(arbitrage.GetExchangeRate("CHF", "CHF") == 1);
        REQUIRE(arbitrage.GetExchangeRate("CHF", "CAD") == 0.953);
    } SECTION("CAD - XXX") {
        REQUIRE(arbitrage.GetExchangeRate("CAD", "USD") == 0.995);
        REQUIRE(arbitrage.GetExchangeRate("CAD", "EUR") == 0.732);
        REQUIRE(arbitrage.GetExchangeRate("CAD", "GBP") == 0.650);
        REQUIRE(arbitrage.GetExchangeRate("CAD", "CHF") == 1.049);
        REQUIRE(arbitrage.GetExchangeRate("CAD", "CAD") == 1);
    }
    // check if the geolocation map is constructed correctly
    SECTION("North America") {
        vector<string> NA = arbitrage.GetCurrencyList("North America");
        REQUIRE(NA.size() == 2);
        REQUIRE(isPresent(NA, "USD"));
        REQUIRE(isPresent(NA, "CAD"));
    } SECTION("Europe") {
        vector<string> EU = arbitrage.GetCurrencyList("Europe");
        REQUIRE(EU.size() == 3);
        REQUIRE(isPresent(EU, "EUR"));
        REQUIRE(isPresent(EU, "GBP"));
        REQUIRE(isPresent(EU, "CHF"));
    }
}

TEST_CASE("Test Arbitrage IsArbitrage", "[is_arbitrage]") {
    // check if the arbitrage_true is detected correctly
    Arbitrage arbitrage_true("exchange_rate_test.csv", "location_test.csv");
    REQUIRE(arbitrage_true.IsArbitrage() == true);

    Arbitrage arbitrage_false("exchange_rate_test2.csv", "location_test.csv");
    REQUIRE(arbitrage_false.IsArbitrage() == false);
}

TEST_CASE("Test Arbitrage GetArbitragePath", "[get_arbitrage]") {
    // check if the arbitrage_true is detected correctly
    Arbitrage arbitrage_true("exchange_rate_test.csv", "location_test.csv");
    vector<vector<string>> path = arbitrage_true.GetArbitrage();

    // check if the path is correct
    vector<string> cycle1 = {"USD", "EUR", "CAD"};
    vector<string> cycle2 = {"GBP", "CHF", "EUR"};
    REQUIRE(hasCycle(path, cycle1) == true);
    REQUIRE(hasCycle(path, cycle2) == true);

    // check if GetArbitrage() returns an empty vector if there is no arbitrage
    Arbitrage arbitrage_false("exchange_rate_test2.csv", "location_test.csv");
    vector<vector<string>> path_2 = arbitrage_false.GetArbitrage();
    REQUIRE(path_2.empty());
}

TEST_CASE("Test GetBetterExchangeRate", "[get_better_exchange_rate]") {
    Arbitrage arbitrage("exchange_rate_test2.csv", "location_test.csv");
    SECTION("normal input") {
        vector<string> path = arbitrage.GetBetterExchangeRate("CHF", "EUR");
        REQUIRE(path.size() == 6);
        REQUIRE(path[0] == "CHF");
        REQUIRE(path[1] == "CAD");
        REQUIRE(path[2] == "GBP");
        REQUIRE(path[3] == "USD");
        REQUIRE(path[4] == "EUR");
        REQUIRE(stod(path[5]) >= arbitrage.GetExchangeRate("CHF", "EUR"));
    } SECTION("invalid input") {
        REQUIRE(arbitrage.GetBetterExchangeRate("CHF", "XXX").empty());
        REQUIRE(arbitrage.GetBetterExchangeRate("XXX", "EUR").empty());
        REQUIRE(arbitrage.GetBetterExchangeRate("XXX", "XXX").empty());
        REQUIRE(arbitrage.GetBetterExchangeRate("", "CHF").empty());
        REQUIRE(arbitrage.GetBetterExchangeRate("CHF", "").empty());
        REQUIRE(arbitrage.GetBetterExchangeRate("", "").empty());
        REQUIRE(arbitrage.GetBetterExchangeRate("CHF", "CHF") == vector<string>{"CHF", "1"});
    }
}

TEST_CASE("Test GetMostValuableCurrency", "[get_most_valuable_currency]") {
    Arbitrage arbitrage("exchange_rate_test.csv", "location_test.csv");
    SECTION("normal input") {
        REQUIRE(arbitrage.GetMostValuableCurrency("USD", {}, DIRECT) == "GBP");
        REQUIRE(arbitrage.GetMostValuableCurrency("USD", {}, BETTER_EXCHANGE_RATE) == "GBP");
        REQUIRE(arbitrage.GetMostValuableCurrency("USD", {"ALL"}, DIRECT) == "GBP");
        REQUIRE(arbitrage.GetMostValuableCurrency("USD", {"North America","ALL"}, DIRECT) == "GBP");
        REQUIRE(arbitrage.GetMostValuableCurrency("USD", {"North America","Europe"}, BETTER_EXCHANGE_RATE) == "GBP");
        REQUIRE(arbitrage.GetMostValuableCurrency("EUR", {"Europe"}, DIRECT) == "GBP");
        REQUIRE(arbitrage.GetMostValuableCurrency("CAD", {"North America"}, DIRECT) == "USD");
    } SECTION("invalid input") {
        REQUIRE(arbitrage.GetMostValuableCurrency("XXX", {}, DIRECT).empty());
        REQUIRE(arbitrage.GetMostValuableCurrency("XXX", {}, BETTER_EXCHANGE_RATE).empty());
        REQUIRE(arbitrage.GetMostValuableCurrency("XXX", {"ALL"}, DIRECT).empty());
        REQUIRE(arbitrage.GetMostValuableCurrency("XXX", {"North America","ALL"}, DIRECT).empty());
        REQUIRE(arbitrage.GetMostValuableCurrency("XXX", {"North America","Europe"}, BETTER_EXCHANGE_RATE).empty());
        REQUIRE(arbitrage.GetMostValuableCurrency("EUR", {"XXX"}, DIRECT).empty());
        REQUIRE(arbitrage.GetMostValuableCurrency("CAD", {"XXX"}, DIRECT).empty());
        REQUIRE(arbitrage.GetMostValuableCurrency("", {}, DIRECT).empty());
        REQUIRE(arbitrage.GetMostValuableCurrency("", {}, BETTER_EXCHANGE_RATE).empty());
        REQUIRE(arbitrage.GetMostValuableCurrency("", {"ALL"}, DIRECT).empty());
        REQUIRE(arbitrage.GetMostValuableCurrency("", {"North America","ALL"}, DIRECT).empty());
        REQUIRE(arbitrage.GetMostValuableCurrency("", {"North America","Europe"}, BETTER_EXCHANGE_RATE).empty());
        REQUIRE(arbitrage.GetMostValuableCurrency("EUR", {""}, DIRECT).empty());
        REQUIRE(arbitrage.GetMostValuableCurrency("CAD", {""}, DIRECT).empty());
    }
}
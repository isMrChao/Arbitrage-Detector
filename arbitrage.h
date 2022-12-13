#ifndef ARBITRAGE_H
#define ARBITRAGE_H

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include "stdc++.h" // added stdc++.h to resolve compatibility issues with macOS

using namespace std;

struct Edge {
    // simple constructor for Edge
    Edge(size_t from, size_t to, double weight) : from(from), to(to), weight(weight) {}
    size_t from;
    size_t to;
    double weight;
};

enum ExchangeMethod {
    DIRECT,
    BETTER_EXCHANGE_RATE
};

class Arbitrage
{
public:
    /**
     * @brief a contructor that takes in the path to the exchange_rate.csv
     * and location.csv and constructs the adjacency matrix and map.
     * There is no output for this function.
     * @param exchange_rate_file : the path to the exchange_rate.csv
     * @param location_file : the path to the location.csv
     */
    Arbitrage(const string& exchange_rate_file, const string& location_file);

    /**
     * @brief a function that has no input and checks if an arbitrage exists
     * by using the negative-cycle detection algorithm (Floyd–Warshall algorithm).
     * If an arbitrage exists, the function will output true (otherwise output false).
     * @return true if an arbitrage exists, false otherwise
     */
    bool IsArbitrage();

    /**
     * @brief a function that has no input and seeks to output a set of conversion strategy to
     * achieve an arbitrage. This feature is done by slightly adjusting the
     * negative cycle detection algorithm (Bellman-Ford algorithm).
     * If arbitrage exists, the function will output a set of conversion strategy to achieve multiple arbitrage.
     * If no arbitrage exists, the function will output an empty vector.
     *
     * This function will call the GetArbitrageHelper() function to get the arbitrage path. After each call, the
     * prohibited_ matrix will be updated to prohibit the path found in the previous call.
     * This function does not seek to find all possible arbitrage paths, as doing so will be a NP-hard problem.
     *
     * @return a vector of vector containing with the name of each currencies to achieve arbitrage as its elements.
     * Empty vector if no arbitrage exists.
     */
    vector<vector<string>> GetArbitrage();

    /**
     * @brief a function that takes in the name of two currencies and outputs the exchange rate
     * Notice: the function will output 0 if there is no exchange rate between the two currencies or
     * if one of the currencies does not exist or if one of the input is empty.
     * @param currencyA : currency from
     * @param currencyB : currency to
     * @return a double representing the exchange rate
     */
    double GetExchangeRate(const string& currencyA, const string& currencyB);

    /**
     * @brief a function that takes in two input - destination currency and source currency.
     * The program will seek for the best strategy (using Bellman-Ford algorithm) to exchange
     * the source currency to the destination currency via a number of intermediate currency.
     * The result strategy will be returned by a vector list with the name of each currency as its element.
     * With the last element being the final exchange rate.
     * Note: if one of the currencies does not exist or empty, the function will output
     * an empty vector. If two currencies are the same, the function will output a vector
     * containing only the currency name and the exchange rate of 1.
     *
     * Since negative cycles exist, the function will seek for a strategy that will not involve any negative cycle
     * using the prohibited_ matrix, which records all of the known edges that exist between the vertices in a some
     * negative cycle. If there is no strategy that does not involve any negative cycle, the function will output
     * the direct conversion rate between the source currency and destination currency.
     * The eventual conversion strategy will be sub-optimal, since the algorithm does not seek for the best strategy.
     * Seeking for the best strategy while working with negative cycle is a NP-Complete problem.
     *
     * @param currency_from : the name of the source currency
     * @param currency_to : the name of the destination currency
     * @return a vector list with the name of each currency as its element and the final exchange rate as the last element.
     */
    vector<string> GetBetterExchangeRate(const string& currency_from, const string& currency_to);

    /**
     * @brief a function that takes in the name of a geolocation and output a list of currencies in that location.
     * If the geolocation does not exist, the function will output an empty vector.
     * @param geolocation : the name of the geolocation
     * @return a vector list with the name of each currency as its element.
     */
    vector<string> GetCurrencyList(const string& geolocation);

    /**
     * @brief  a function that has two input - the name of the reference currency and geographical filter (if the input
     * vector is empty, the function will then traverse the whole graph). The function will traverse through all of the
     * other currencies (using Breadth First Search) under the constraint of geo_filter to find the one that worth the
     * most value in terms of how much 1 unit of that currency worth in the reference currency. The user is able to use
     * different exchange method to find the best currency.
     * @param reference_currency
     * @param geo_filter
     * @return the name of the most valuable currency in terms of how much 1 unit of that currency worth in the
     * reference currency.
     */
    string GetMostValuableCurrency(const string& reference_currency, const vector<string>& geo_filter,
                                   ExchangeMethod method);



private:
    vector<vector<double>> adjacency_matrix_;
    vector<vector<double>> adjacency_matrix_negative_log_;  // negative log of the edge weight (Floyd-Warshall algorithm)
    vector<Edge> edges_negative_log_;                       // negative log of the edge weight (Bellman-Ford algorithm)
    map<string, size_t> currency_index_;                    // map from currency name to index (or id)
    map<size_t, string> index_currency_;                    // map from index (or id) to currency name
    map<string, vector<size_t>> geo_map_;                   // map from geolocation name to a vector of currency index
    vector<vector<bool>> prohibited_;                       // a matrix that records all prohibited conversions

    /**
     * @brief private helper function to help locate the negative cycle
     * The function will run the Bellman-Ford algorithm and return the negative cycle if it exists.
     * The returned negative cycle will also be recorded in the prohibited_ for future use.
     * @return
     */
    vector<string> GetArbitrageHelper();
};

#endif //ARBITRAGE_H

#ifndef ARBITRAGE_H
#define ARBITRAGE_H

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <bits/stdc++.h>

using namespace std;

struct Edge {
    // simple constructor for Edge
    Edge(size_t from, size_t to, double weight) : from(from), to(to), weight(weight) {}
    size_t from;
    size_t to;
    double weight;
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
     * @brief a function that has no input and seeks to output a set of convertion strategy to
     * achieve an arbitrage. This feature is done by slightly adjucting the
     * negative cycle detection algorithm (Bellman-Ford algorithm).
     * If an arbitrage exists, the function will output a vector containing with the name of
     * each currencies to achieve arbitrage as its elements.
     * If no arbitrage exists, the function will output an empty vector.
     * @return a vector containing with the name of each currencies to achieve arbitrage as its elements.
     * Empty vector if no arbitrage exists.
     */
    vector<string> GetArbitrage();

    /**
     * @brief a function that takes in the name of two currencies and outputs the exchange rate
     * Notice: the function will output 0 if there is no exchange rate between the two currencies or
     * if one of the currencies does not exist.
     * @param currencyA : currency from
     * @param currencyB : currency to
     * @return a double representing the exchange rate
     */
    double GetExchangeRate(const string& currencyA, const string& currencyB);

private:
    vector<vector<double>> adjacency_matrix_;
    vector<vector<double>> adjacency_matrix_negative_log_;  // negative log of the edge weight (Floyd-Warshall algorithm)
    vector<Edge> edges_negative_log_;                       // negative log of the edge weight (Bellman-Ford algorithm)
    map<string, size_t> currency_index_;                    // map from currency name to index (or id)
    map<size_t, string> index_currency_;                    // map from index (or id) to currency name
};

#endif //ARBITRAGE_H

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
     * @return a vector containing with the name of each currencies to achieve arbitrage as its elements
     */
    vector<string> GetArbitrage();

    /**
     * @brief a function that takes in the name of two currencies and outputs the exchange rate
     * Notice: the function will output 0 if there is no exchange rate between the two currencies or
     * if one of the currencies does not exist.
     * @param currencyA
     * @param currencyB
     * @return a double representing the exchange rate
     */
    double GetExchangeRate(const string& currencyA, const string& currencyB);

private:
    vector<vector<double>> adjacency_matrix_;
    vector<vector<double>> adjacency_matrix_negative_log_;
    map<string, size_t> currency_index_;
};

#endif //ARBITRAGE_H

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
     * if one of the currencies does not exist.
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
     * Note: if two currencies are the same or one of the currencies does not exist, the function will output
     * an empty vector.
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
    vector<string> FindBestExchangeRate(const string& currency_from, const string& currency_to);

private:
    vector<vector<double>> adjacency_matrix_;
    vector<vector<double>> adjacency_matrix_negative_log_;  // negative log of the edge weight (Floyd-Warshall algorithm)
    vector<Edge> edges_negative_log_;                       // negative log of the edge weight (Bellman-Ford algorithm)
    map<string, size_t> currency_index_;                    // map from currency name to index (or id)
    map<size_t, string> index_currency_;                    // map from index (or id) to currency name
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

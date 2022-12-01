#include "arbitrage.h"

Arbitrage::Arbitrage(const string& exchange_rate_file, const string& location_file) {
    // open the exchange_rate.csv file
    fstream exchange_rate(exchange_rate_file);
    // exit the program if the file cannot be opened
    if (!exchange_rate.is_open()) {
        cout << "Error opening exchange_rate.csv" << endl;
        exit(1);
    }
    // use a map to store the currency index and their exchange rate
    map<pair<size_t, size_t>, double> exchange_rate_map;
    size_t current_index = 0;
    while (!exchange_rate.eof()) {
        string line;
        string currencyA;
        string currencyB;
        size_t indexA;
        size_t indexB;
        double rate;
        getline(exchange_rate, line);

        // parse the line
        stringstream ss(line);
        getline(ss, currencyA, ',');
        currencyB = currencyA.substr(4, currencyA.length() - 1);
        currencyA = currencyA.substr(0, 3);
        ss >> rate;

        // add the currency to the map if it is not in the map
        if (currency_index_.find(currencyA) == currency_index_.end()) {
            currency_index_[currencyA] = current_index;
            current_index++;
        }
        if (currency_index_.find(currencyB) == currency_index_.end()) {
            currency_index_[currencyB] = current_index;
            current_index++;
        }
        // get the index of the currency
        indexA = currency_index_[currencyA];
        indexB = currency_index_[currencyB];
        exchange_rate_map[make_pair(indexA, indexB)] = rate;
    }
    exchange_rate.close();
    //initialize the adjacency matrix
    adjacency_matrix_ = vector<vector<double>>(currency_index_.size(), vector<double>(currency_index_.size(), 0));
    adjacency_matrix_negative_log_ = vector<vector<double>>(currency_index_.size(), vector<double>(currency_index_.size(), 0));
    // fill the adjacency matrix
    for (auto & it : currency_index_) {
        adjacency_matrix_[it.second][it.second] = 1;
        adjacency_matrix_negative_log_[it.second][it.second] = 0;
    }
    for (auto & it : exchange_rate_map) {
        adjacency_matrix_[it.first.first][it.first.second] = it.second;
        adjacency_matrix_negative_log_[it.first.first][it.first.second] = -log(it.second);
    }

    // TODO: read the location.csv file and construct the geo location map
}

bool Arbitrage::IsArbitrage() {
    // find negative cycle using Floyd-Warshall algorithm
    vector<vector<double>> distance = adjacency_matrix_negative_log_;
    for (size_t k = 0; k < distance.size(); k++) {
        for (size_t i = 0; i < distance.size(); i++) {
            for (size_t j = 0; j < distance.size(); j++) {
                if (distance[i][k] + distance[k][j] < distance[i][j]) {
                    distance[i][j] = distance[i][k] + distance[k][j];
                }
            }
        }
    }
    // check if there is a negative cycle
    for (size_t i = 0; i < distance.size(); i++) {
        if (distance[i][i] < 0) {
            return true;
        }
    }
    return false;
}

vector<string> Arbitrage::GetArbitrage() {
    // TODO
    return vector<string>();
}

double Arbitrage::GetExchangeRate(const string &currencyA, const string &currencyB) {
    // check for invalid currency input
    if (currencyA == currencyB) return 1;
    if (currency_index_.find(currencyA) == currency_index_.end() || currency_index_.find(currencyB) == currency_index_.end()) return 0;

    // retrieve the exchange rate from the adjacency matrix
    return adjacency_matrix_[currency_index_[currencyA]][currency_index_[currencyB]];
}

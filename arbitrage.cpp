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
            index_currency_[current_index] = currencyA;
            current_index++;
        }
        if (currency_index_.find(currencyB) == currency_index_.end()) {
            currency_index_[currencyB] = current_index;
            index_currency_[current_index] = currencyB;
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
        edges_negative_log_.emplace_back(it.second, it.second, 0);
    }
    for (auto & it : exchange_rate_map) {
        adjacency_matrix_[it.first.first][it.first.second] = it.second;
        adjacency_matrix_negative_log_[it.first.first][it.first.second] = -log(it.second);
        edges_negative_log_.emplace_back(it.first.first, it.first.second, -log(it.second));
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
    // print the arbitrage path using Bellman-Ford algorithm
    size_t vertex_num = currency_index_.size();
    size_t edge_num = edges_negative_log_.size();

    // initialize the distance vector and the parent vector
    // the distance vector stores the shortest distance from the source to each vertex
    // the parent vector stores the parent of each vertex
    // the distance vector is initialized to infinity
    // the parent vector is initialized to -1
    vector<double> distance(vertex_num, numeric_limits<double>::max());
    vector<size_t> parent(vertex_num, -1); // parent of each vertex

    // set the source to be the first vertex
    distance[0] = 0;
    parent[0] = 0;

    // relax the edges (vertex_num - 1) times
    bool flag = true;
    for (size_t i = 1; i <= vertex_num - 1; i++) {
        if (!flag) break;
        flag = false;
        for (size_t j = 0; j < edge_num; j++) {
            size_t from = edges_negative_log_[j].from;
            size_t to = edges_negative_log_[j].to;
            double weight = edges_negative_log_[j].weight;
            if (distance[from] != numeric_limits<double>::max()
                && distance[from] + weight < distance[to]) {
                flag = true;
                distance[to] = distance[from] + weight;
                parent[to] = from;
            }
        }
    }

    // check if there is a negative cycle
    size_t negative_cycle_vertex = -1;
    for (size_t i = 0; i < edge_num; i++) {
        size_t from = edges_negative_log_[i].from;
        size_t to = edges_negative_log_[i].to;
        double weight = edges_negative_log_[i].weight;
        if (distance[from] != numeric_limits<double>::max()
            && distance[from] + weight < distance[to]) {
            negative_cycle_vertex = to;
            break;
        }
    }

    // if there is a negative cycle, print the arbitrage path
    if (negative_cycle_vertex != -1) {
        vector<string> arbitrage_path;
        for (size_t i = 0; i < vertex_num; i++) {
            negative_cycle_vertex = parent[negative_cycle_vertex];
        }
        size_t start = negative_cycle_vertex;
        arbitrage_path.push_back(index_currency_[start]);
        while (parent[negative_cycle_vertex] != start) {
            negative_cycle_vertex = parent[negative_cycle_vertex];
            arbitrage_path.push_back(index_currency_[negative_cycle_vertex]);
        }
        arbitrage_path.push_back(index_currency_[start]);
        // reverse the arbitrage path
        reverse(arbitrage_path.begin(), arbitrage_path.end());
        return arbitrage_path;
    } else {
        // if there is no negative cycle, return an empty vector
        return {};
    }
}

double Arbitrage::GetExchangeRate(const string &currencyA, const string &currencyB) {
    // check for invalid currency input
    if (currencyA == currencyB) return 1;
    if (currency_index_.find(currencyA) == currency_index_.end() || currency_index_.find(currencyB) == currency_index_.end()) return 0;

    // retrieve the exchange rate from the adjacency matrix
    return adjacency_matrix_[currency_index_[currencyA]][currency_index_[currencyB]];
}

vector<string> Arbitrage::FindBestExchangeRate(const string &currency_from, const string &currency_to) {

    return vector<string>();

}

![This is an image](https://raw.githubusercontent.com/isMrChao/cs225final/main/Header.png)
# Brief Introduction
This is a project for CS225 Introduction to Data Structures and Algorithms. This project is aimed to find arbitrage opportunities in the currency exchange market, in other words, to generate profit by buying and selling currencies. Users can input their own currency exchange rates as .csv files and the program will seek for any arbitrage opportunities by using negative cycle detection algorithm.

# Installation
**Note: .csv files are located in the csv_files folder. Before running the program, please make sure you update the file path in the main.cpp file and test.cpp file.**

### Clion
For the best compatibility, please use **CLion** to install the program by simply select: 
```
File -> New -> Project from Version Control -> Repository URL: https://github.com/isMrChao/cs225final.git
```
- The main executable can be run by selecting Final_Project from the drop down menu in the top right corner of the CLion window. The test executable can be run by selecting Test_Final_Project from the drop down menu in the top right corner of the CLion window. If Test_Final_Project is not available, please select:
```
Edit Configurations -> CMakeApplication -> Add new run configuration -> Name: Test_Final_Project -> Target: Test_Final_Project -> OK
```
- ~~Due to the slow link speed of MinGW (default compiler in CLion), the test executable may take a long time to compile with Catch2 after making changes to the code.~~ Catch2 test environment has been updated to 3.0. Now expect faster compilation time.

### Visual Studio Code
For **Visual Studio Code**, please install the related extensions and clone the repository to your local machine. After cloning, please change the working directory to the root directory of the project and run the following commands:
```
mkdir build
cd build
```
- After creating the build directory, please run the following commands:
```
cmake ..
make
```
- The main executable can be run by running the following command:
```
./Final_Project
```
- The test executable can be run by running the following command:
```
./Test_Final_Project
```



# Presentation
- [YouTube Page](https://youtu.be/DG8e_1KOylU)

# Written Report

In the proposal, we stated the leading question of this project. That is, to develop a method in making money by simply converting currencies from one to another. Now we can honestly say that the goal is achieved.

Several changes have made to the original proposal:
- The GetArbitrage function is now able to output most arbitrage cycles in the currency exchange market. The original proposal only required the function to output one arbitrage cycle. 
- GetBestExchangeRate function is now re-named to GetBetterExchangeRate due to the limitation on the Bellman-Ford shortest path algorithm when the graph has negative cycles. The current implementation of GetBetterExchangeRate is able to work with negative cycles, but the output is not guaranteed to be the best exchange rate. 
- Users can now select the exchange method (DIRECT using GetExchangeRate and BETTER_EXCHANGE_RATE using GetBetterExchangeRate) when using the GetMostValuableCurrency function. The original proposal only required the function to output the most valuable currency using the DIRECT exchange method.

Below is the technical details of our implementation:

For the exchange rate data, we used Excel to retrieve the exchange rates between currencies and stored them as exchange_rate.csv file. We implemented two adjacency matrices and one adjacency list with each one of them serving a different purpose to store the weighted graph converted from the input dataset.
(The first adjacency matrix is used to store the direct exchange rates between currencies, the second adjacency matrix stores the negative log of the direct exchange rates that can be used by Floyd–Warshall algorithm to detect arbitrage, and the adjacency list also stores the negative log of the direct exchange rates but is used by Bellman-Ford algorithm to detect the exact arbitrage cycle.)

For the geolocation data, we used Python to retrieve the geolocation of each currency and stored them as location.csv file. We implemented a hash table to store the geolocation of each currency. The hash table maps the geolocation name (e.g. East Asia, North America, Europe) to the list of currencies that belong to that location. 

The constructor will set up everything that is needed for above-mentioned data structures. Aside from that, we have another matrix stored as a private variable that keeps a record for all prohibited conversions (or edges in the weighted graph). This allows the program to detect new arbitrage cycles after the last arbitrage cycle is detected, and also allows the FindBetterExchangeRate function to work with the presence of negative cycles. This matrix only initialized when the GetArbitrage or FindBetterExchangeRate function is called.

The main functionality is supported by the Get Arbitrage function, which returns a vector of vector containing with the name of each currency to achieve arbitrage as its elements. (Or an empty vector when no arbitrage is found). We implemented a modified version of Bellman-Ford algorithm. To be more specific, the return value of the function can be seen as the steps to convert currencies in a vector list with the name of each currency as its element. For the actual implementation of this function, we found that finding all negative cycles in a directed graph with negative edges is a NP-Hard problem, meaning that existing algorithms are not efficient enough to solve this problem in a reasonable amount of time. Therefore, we decided to use a modified version of Bellman-Ford algorithm to detect most of the arbitrage cycles in the graph.

Besides this function, we've also encountered a NP-Complete problem when implementing the FindBetterExchangeRate (previously called FindBestExchangeRate) function. The problem is given a source and destination currencies, we need to find a more efficient strategy to convert currencies from the source to the destination by involving several intermediate currencies. Under ideal circumstances, the function will output a strategy which guarantees a better overall exchange rate than doing the conversion directly. However, given the fact that negative cycles exist in our graph, traditional shortest path algorithms will not work as when a negative cycle is involved, the shortest path will be negative infinite. As we researched potential ways to solve this, we found that finding the best exchange strategy while dealing with negative cycles is NP-Complete. We eventually decided to use the prohibited matrix from the GetArbitrage function to solve this problem. Any edges/conversions that are involved in the negative/arbitrage cycle will be marked as prohibited, and the shortest path algorithm will not consider these edges. This allows us to find the shortest path without the presence of negative cycles. Although using such measures to find the exchange strategy can yield some good results, those results are not guaranteed to be the best.

The above functionality fulfills our original goal written in the proposal. Other than that, we have developed some extra features for our project. Such as finding the most valuable currency in the perspective of the reference currency given the range of the geographical location.

Given the aforementioned NP issues we had to deal with, some other minor problems are also encountered during the implementation. For example, as we originally planned to retrieved our exchange_rate data from the online public source, we found that those datasets are either very limited (only containing a handful currencies and their exchange rates) or do not offer the amount of customization we need. Other methods such as acquiring exchange rates via Python API also failed as such API requires a paid subscription. Therefore, we decided to use Excel to retrieve the exchange rates (supported by Bing) and store them in a .csv file. This method is not ideal as it requires manual work to update the exchange rates. However, it is the best solution we can come up with given our budget and time constraint.

Beside from that, setting up the test environment is also a challenge. We tried to use the Catch2 framework to set up the test environment, but our IDE platform CLion did not provide much support for setting up the test environment. There is little to no documentation on JetBrain's website that provides a clear tutorial on how to set up the configuration. It appeared to us that Google Test framework is much more favored by modern IDE platforms. When we eventually managed to set up Catch2, we found that the compilation time is very slow. This is potentially due to the fact that Catch2 is a header-only library, meaning that the compiler needs to compile the entire library every time we make changes to the code. We eventually found a solution to this problem by migrating to the 3.0 version of Catch2. This version of Catch2 is no longer a header-only library, and the compilation time is much faster.
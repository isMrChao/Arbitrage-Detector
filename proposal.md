*n stands for the number of currencies
## Leading Question 
How to make money by simply converting currencies from one to another?

- In modern financial markets, different conversion rates exist between currencies. There are often small deviations between these rates. A skilled investor can often make extra profit from these small deviations, which we call arbitrage. After doing the research, we found that if currencies are treated as vertex on a graph and the exchange rates between them are transformed into weighted edges, we can use the existing graph algorithm to detect whether arbitrage is possible at the current exchange rate.
## Dataset Acquisition
We plan to use Excel to obtain the exchange rates between different currencies in real time (Excel's live currency capture feature is supported by Bing). The live rates will be saved as a .csv file. In order to save computing resources, we intend to obtain the exchange rates of the top 50 currencies in the world.
## Data Format
The exchange rates dataset will be stored in a .csv file.
## Data Correction
The default weight for establishing a connecting edge is 0. When a empty entry or statistical unrealistic value is detected. The default value will be used to establish the edge.

## Data Storage
We plan to use an adjacency matrix to store the weighted graph converted from the input dataset. The estimated storage cost for this data structure will be O(n^2).

## Algorithm
- Constructor: a contructor that takes in the path to the dataset.csv and constructs the adjacency matrix. There is no output for this function. The time complexity for this function is estimated to be O(n^2). The space complexity for adjacency matrix is estimated to be O(n^2).

- Find Arbitrage: a function that has no input and checks if an arbitrage exists by using the negative-cycle detection algorithm (Bellman-Ford Algorithm). If an arbitrage exists, the function will output the steps to convert currencies in a vector list with the name of each currency as its element. The time complexity of this function is estimated to be O(n^2*(n-1)/2). The space complexity for this function is estimated to be O(n*(n-1)/2)

## Timeline

    Week 1 (10/28 - 11/4) Complete team contract Complete final project proposal Create a Github repo
    Week 2 (11/4 - 11/11)
    Identify all the functions we need to accomplish
    Decide which algorithm to use
    Discuss together and set up the frame of the final project (classes, member functions, private variables...)
    Week 3 (11/11 - 11/18)
    Write test cases
    Write part of the member functions
    Week 4 (11/18 - 11/25)
    Finish writing all of the member functions
    Should be able to run all tests
    Final debug, make sure the code works on edge cases
    Week 5 (11/25-12/2)
    Analyze the efficiency of the code Complete the README. file Complete the written report
    Week 6 (12/2 - 12/8) Complete final representation Final check and submission

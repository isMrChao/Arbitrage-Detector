# CS 225 Final: Arbitrage Detector
- For the best compatibility, please use **CLion** to install the program by simply select: 
```
File -> New -> Project from Version Control -> Repository URL: https://github.com/isMrChao/cs225final.git
```
- The main executable can be run by selecting Final_Project from the drop down menu in the top right corner of the CLion window. The test executable can be run by selecting Test_Final_Project from the drop down menu in the top right corner of the CLion window.
- ~~Due to the slow link speed of MinGW (default compiler in CLion), the test executable may take a long time to compile with Catch2 after making changes to the code.~~ Catch2 test environment has been updated to 3.0. Now expect faster compilation time.
- Note: .csv files are located in the cmake-build-debug folder
# Written Report

In the proposal, we stated the leading question of this project. That is, to develop a method in making money by simply converting currencies from one to another. Now we can honestly say that the goal is achieved.

As for the actual practice, we used Excel to retrieve the exchange rates between currencies, and stored them as csv files. We implemented an adjacency matrix to store the weighted graph converted from the input dataset. 

The constructor will set up the adjacency matrix and everything that is needed for the matrix. Aside from that, we have another matrix stored as a private variable that keeps a record for all prohibited conversions, which allow the function to perform more advanced calculations. 

The main functionality is supported by the Get Arbitrage function, which returns a vector of vector containing with the name of each currencies to achieve arbitrage as its elements. (Or an empty vector when no arbitrage is found). We implemented the Bellman-Ford algorithm. To be more specific, the return value of the function can be seen as the steps to convert currencies in a vector list with the name of each currency as its element.

The above functionality fulfills our original goal written in the proposal. Other than that, we have developed some extra features for our project. Such as finding arbitrage opportunity with a restriction on the continents(such as America, Asia, Europe), and so on.

During the process, we encountered certain problems and managed to solve them. We will just shed lights on one of them here, since the rest is already recorded within development logs. The conversion between CSV files and Excel files do not guarantee the safety of the original format. For instance, if we only selected 5 currencies for exchange, the excel file will look just good, but while turning the data into a csv file or store to a vector, there could be certain number of empty elements lingering in the back of the vector (the amount depending on the default value set by Excel). We fixed this by addressing those conversions as if they were disturbed and ensuring no invalid elements of our vectors would be misused.

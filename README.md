# Riot Api Client in Cpp

Basic api alient for RIOT GAMES API. Currently handles rate limiting, 5XX error codes and basic logging functionality. Only implements league of legends queries currently and in the process of adding and testing all other queries.

Currently in development. 

Uses libcurl to send requests and jsoncpp to store return values. Shared object files and lib can be found in the build/src directory. In the process of documenting usage and making the library more accessible. If you find any error or want to suggest some changes, feel free to make a pull request.

## To do:

Short Term
* Add all possible queries
* Create tests for all queries
* Move neural network class

Long Term
* Add higher level queries (All, csv conversion?)
* Python wrapper?

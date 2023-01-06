# Riot Api Client in Cpp

Basic api alient for RIOT GAMES API. Currently handles rate limiting, 5XX error codes and basic logging functionality. Only implements league of legends queries currently and in the process of adding and testing all other queries.

Currently in development. 

Uses libcurl to send requests and jsoncpp to store return values. Shared object files and lib can be found in the build/src directory. In the process of documenting usage and making the library more accessible. If you find any error or want to suggest some changes, feel free to make a pull request.

## Usage

Currently incorrect names of endpoints will cause an exception, I will write a more informative exception

```Cpp
#include <jsoncpp>
#include <string>

int main() {
    
    std::string PATH_TO_API_KEY, PATH_TO_LOG_FILE;
    bool log_all, overwrite_logs;
    client::RiotApiClient example_client(PATH_TO_API_KEY, PATH_TO_LOG_FILE, log_all, overwrite_logs);
    
    Json::Value response;
    std::string endpoint = "SUMMONER-V4";
    std::string method = "by-name";

    std::vector<std::string> params = {"Kr", "Hide on bush"}; // region, params...

    response = example_client.query(endpoint, method,  params);
}
```

## Including API Key

Pass path to file to client constructor.

```Json
{
    "api-key" : "<API KEY>"
}
```

## Example Log 

Used invalid api_key

```txt
--- Client Initialised Started ---
Time initialised: Sunday Sun Jan  1 17:27:20 2023
Path to log file: ../test/test_log.txt
overwite: true, log_all queries: false
Retrieved API key
Curl initialised
MAX SERVICE DENIALS (503): 2 
MAX INTERNAL SERVICE ERRORS (500): 2 
--- Client Successfully Initialised ---
 
--- NEW QUERY --- 
Address_sent: https://asia.api.riotgames.com/lol/match/v5/matches/by-puuid/6dgDp5y88RxqOmVMv1GRoGaCmPP-uAbmlsVRhKQj4g0KdIH_GxqCEE6w0JRmHRxSTzbtxMFGypJZIg/ids
Time finished: Sunday Sun Jan  1 17:27:21 2023
Final Response Code: 403
Final Response Message: Forbidden
Attempts: rate_denials: 0, internal_errors: 0, service_denials: 0 
 
--- NEW QUERY --- 
Address_sent: https://asia.api.riotgames.com/lol/match/v5/matches/KR_6279823690
Time finished: Sunday Sun Jan  1 17:27:21 2023
Final Response Code: 403
Final Response Message: Forbidden
Attempts: rate_denials: 0, internal_errors: 0, service_denials: 0
```

## To do:

Short Term
<<<<<<< HEAD
* Create tests for all queries
=======
* Handle 5XX better
* Add more flexible rate handling (developer, app, ...)
* Create/Re-write tests for all queries
* Document all query methods
>>>>>>> restructuring

Long Term
* Add higher level queries (All, csv conversion?)
* Python wrapper?

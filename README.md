# Riot Api Client in Cpp

Basic api alient for RIOT GAMES API. Currently handles rate limiting and 5XX error codes.

Currently in development. 

Uses libcurl to send requests and jsoncpp to store return values. Shared object files and lib can be found in the build/src directory. In the process of documenting usage and making the library more accessible. If you find any error or want to suggest some changes, feel free to make a pull request.

## Usage

Currently incorrect names of endpoints will cause an exception, I will write a more informative exception

```Cpp
#include <jsoncpp/json/json.h>
#include <string>

using required_args_type = std::vector<std::string>;
using optional_args_type = std::vector<std::pair<std::string, std::string>>;

int main() {
    
    std::string PATH_TO_API_KEY;
    client::RiotApiClient example_client(PATH_TO_API_KEY);
    
    Json::Value response;
    std::string endpoint = "MATCH-V5";
    std::string method = "by-puuid";

    required_args_type params = {"<routing_here>", "<puuid_here>"}; // region, params...
    optional_args_type optional_arguments = {{"startTime", "<user_arg>"}, {"endTime", <user_arg>}, ...}  

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

## Testing

Testing of most endpoints is located in the test directory. Currently, my basic development key does not giving access to all endpoints to test. 

## To do/How to contribute:

All contribution are welcome, see the issues of the repository to see what you can work on or suggest something else you think might be interesting.

Create a pull request when you're ready.


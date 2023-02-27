# Riot Api Client in Cpp

Basic api alient for RIOT GAMES API. Currently handles rate limiting, response errors and logging.

Currently in development. 

Uses libcurl to send requests and jsoncpp to store return values. In the process of documenting usage and making the library more accessible. If you find any error or want to suggest some changes, feel free to make a pull request.

## Usage

```Cpp
#include <jsoncpp/json/json.h>
#include "path/to/client.h"

int main() {
    
    client::RiotApiClient example_client("<path/to/api/key>", "<path/to/logging/file>", logging::LEVEL::<level>, <bool verbose>);
    Json::Value response;

    response = example_client.Match.by_puuid("routing", "puuid", {"startTime", <user_arg>}, {"endTime", <user_arg>}, ...);
}
```

NOTE: optional arguements that are passed as string must be first

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


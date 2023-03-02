### Basic Usage

Below demonstrates a request to Match V5 endpoint using PUUID with optional arguements. All endpoints return a Json::Value object.

```Cpp
#include <jsoncpp/json/json.h>
#include "path/to/client.h"

int main() {
    
    client::RiotApiClient example_client("<path/to/api/key>", "<path/to/logging/file>", logging::LEVEL::<level>, <bool verbose>);
    Json::Value response;

    response = example_client.Match.by_puuid("routing", "puuid", {"startTime", <user_arg>}, {"endTime", <user_arg>}, ...);
}
```

### Including your API Key

It is highly recommended not to included api keys in source code as one may unintentionally share source coded publically with the api key exposed.

Riot-cpp accepts a path to a json file to extract your api key. The file should be of the following format.

```Json
{
    "api-key" : "<API KEY>"
}
```

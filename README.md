# Welcome to Riot-cpp

Simple Api client for the Riot Games Resful API implemented in C++ 20. 

This library implements rate limiting to prevent api key blacklisting from exceeding Riot's rate limits. The library handles server response error's and will retry when a successful request is possible (429, 500, 503 errors). The client contains a logging class to help with debugging dependent code as well as providing information of the running of the client. Riot-cpp will adapt to the specific api key's rate limits after the first request and try to recover from errors.

Riot-cpp uses [libcurl](https://curl.se/libcurl/) to send https get requests and [jsoncpp](https://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html) to handle json data structure encoding and json file parsing.

Documentation is located at following page 

[dan-tan.github.io/riot-cpp](https://dan-tan.github.io/riot-cpp/)

# Basic Usage

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

## Including your API Key

It is highly recommended not to included api keys in source code as one may unintentionally share source coded publically with the api key exposed.

Riot-cpp accepts a path to a json file to extract your api key. The file should be of the following format.

```Json
{
    "api-key" : "<API KEY>"
}
```

## Installation

Currently the repository must be cloned or downloaded via the releases.

```git
git clone git+https://github.com/Dan-Tan/riot-cpp.git
```
Option 1: CMake

Linking to executable or library

```cmake
add_subdirectory(<path/to/riot-cpp/src>)
target_link_libraries(<your library or executable> <PRIVATE/PUBLIC> riot-cpp)
```

Option 2: Manually with libriot-cpp.

The shared object file is located as $riot-cpp/build/src/libriot-cpp.so$ and can be manually linked with your compiler of choice. You must include the following in your includes

```cpp
#include "path/to/riot-cpp/src/client/client.h"
```

See options for linking with gcc, [Options for Linking](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html)

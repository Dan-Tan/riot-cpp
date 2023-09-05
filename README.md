# Welcome to Riot-cpp

[![ci](https://github.com/Dan-Tan/riot-cpp/workflows/ci/badge.svg)](https://github.com/Dan-Tan/riot-cpp/actions?query=workflow:"ci")
[![build](https://github.com/Dan-Tan/riot-cpp/workflows/build/badge.svg)](https://github.com/Dan-Tan/riot-cpp/actions?query=workflow:"build")

Simple Api client for the Riot Games Resful API implemented in C++ 20. 

This library implements rate limiting to prevent api key blacklisting from exceeding Riot's rate limits. The library handles server response error's and will retry when a successful request is possible (429, 500, 503 errors). The client contains a logging class to help with debugging dependent code as well as providing information of the running of the client. Riot-cpp will adapt to the specific api key's rate limits after the first request and try to recover from errors.

Riot-cpp uses [libcurl](https://curl.se/libcurl/) to send https get requests. The Json parser [simdjson](https://github.com/simdjson/simdjson) is used as a dependency for testing but NOT in the library. Parsing is done by the user.

Documentation is located at following page.

[![view - Documentation](https://img.shields.io/badge/view-Documentation-blue?style=for-the-badge)](https://dan-tan.github.io/riot-cpp/)

# Basic Usage

Below demonstrates a request to Match V5 endpoint using PUUID with optional arguements. All endpoints return a unique pointer to a vector of chars. Dependency on Jsoncpp or an individual Json library was remoed to allow the user to decide how they wanted to parse json, allows flexibility for users if performance is a key concern for large responses.

```Cpp
#include "path/to/client.h"

using json_ptr = std::unique_ptr<std::vector<char>>

int main() {
    
    client::RiotApiClient example_client("<path/to/api/key>", "<path/to/logging/file>", logging::LEVEL::<level>, <bool verbose>);

    json_ptr response = example_client.Match.by_puuid("routing", "puuid", {"startTime", <user_arg>}, {"endTime", <user_arg>}, ...);
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
Techincally, as long as "RGAPI-..." is anywhere in the file the client will find it. 

## Installation

Currently the repository must be cloned or downloaded via the releases.

```bash
git clone git+https://github.com/Dan-Tan/riot-cpp.git
```

Git submodules are highly recommended for use as a project dependency.

```bash
git submodule add git+https://github.com/Dan-Tan/riot-cpp.git
```
### Option 1: CMake

Linking to executable or library

```cmake
add_subdirectory(<path/to/riot-cpp/src>)
target_include_directories(<your library or executable> <PRIVATE/PUBLIC> <path/to/riot-cpp/src/riot-cpp>)
target_link_libraries(<your library or executable> <PRIVATE/PUBLIC> riot-cpp)
```

### Option 2: Manually with libriot-cpp.

The shared object file is located as "riot-cpp/build/src/riot-cpp/libriot-cpp.so" and can be manually linked with your compiler of choice. You must include the following in your includes

```cpp
#include "path/to/riot-cpp/src/client/client.h"
```

See options for linking with gcc, [Options for Linking](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html)

## Disclaimer

Riot-cpp is not developed by or affiliated with Riot Games. 

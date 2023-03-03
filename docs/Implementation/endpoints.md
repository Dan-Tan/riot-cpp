# Endpoints
Riot-cpp implements all offical riot api endpoints not including the following
* LOR-DECK-V1
* LOR-INVECTORY-V1
* TOURNAMENT-V4
* TOURNAMENT-STUB-V4

The following code assumes the client has already been initialised as follows
```cpp
// #include "path/to/client.h"

client::RiotApiClient client_obj{"<path/to/api-key.json>", // std::string
                                 "<path/to/logfile>", // std::string
                                 logging::LEVEL::<level>, // logging::LEVEL
                                 verbosity // bool
                                 }
```

### ACCOUNT-V1

#### Usage

```cpp
// by-puuid
Json::Value result = client_obj.Account.by_puuid("<routing>", "puuid");
```



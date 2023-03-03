### MATCH-V5

View [Riot's](https://developer.riotgames.com/apis#match-v5) documentation.

**by-puuid**

Querying MATCH-V5 using puuid accepts a large number of optional arguments. This method allows for all optional arguments, however, the string argument *type* must be passed first. Currently, *type* must be used if other optional fields are being used (I will overload to solve this soon).
```cpp
// function call
Json::Value response = client_obj.Match.by_puuid("<routing>", "<puuid>", {"optional_key", <optional_arg>}, ...);
// declaration
Json::Value by_puuid(std::string, std::string, std::pair<std::string, Arg_Type>, ...);
```
**by-match-id**
```cpp
// function call
Json::Value response = client_obj.Match.by_match_id("<routing>", "<match-id>");
// declaration
Json::Value by_match_id(std::string, std::string);
```
**timeline**
```cpp
// function call
Json::Value response = client_obj.Match.timeline("<routing>", "<match-id>");
// declaration
Json::Value timeline(std::string, std::string);
```

### ACCOUNT-V1

View [Riot's](https://developer.riotgames.com/apis#account-v1) documentation of ACCOUNT-V1

**by-puuid**
```cpp
// function call
Json::Value response = client_obj.Account.by_puuid("<routing>", "<puuid>");
// declaration
Json::Value
```
**by-riot-id**
```cpp
// function call
Json::Value response = client_obj.Account.by_riot_id("<routing>", "<gameName>", "<tagline>");
// declaration
Json::Value by_riot_id(std::string, std::string, std::string);
```
**by-game**
```cpp
// function call
Json::Value response = client_obj.Account.by_game("<routing>", "<game>", "<puuid>");
// declaration
Json::Value by_game(std::string, std::string, std::string);
```

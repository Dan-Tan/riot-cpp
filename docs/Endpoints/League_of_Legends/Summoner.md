### SUMMONER-V4

View [Riot's](https://developer.riotgames.com/apis#summoner-v4) documentation of SUMMONER-V4.


**by-account-id**
```cpp
// function call
Json::Value response = client_obj.Summoner.by_account_id("<routing>", "<account-id>")
// declaration
Json::Value by_account_id(std::string, std::string);
```
**by-name**
```cpp
// function call
Json::Value response = client_obj.Summoner.by_name("<routing>", "<name>");
// declaration
Json::Value by_name(std::string, std::string);
```
**by-puuid**
```cpp
// function call
Json::Value response = client_obj.Summoner.by_puuid("<routing>", "<puuid>");
// declaration
Json::Value by_puuid(std::string, std::string);
```
**by-summoner-id**
```cpp
// function call
Json::Value response = client_obj.Summoner.by_summoner_id("<routing>", "<summoner-id>");
// declaration
Json::Value by_summoner_id(std::string, std::string);
```
**by-rso-puuid**
```cpp
// function call
Json::Value response = client_obj.Summoner.by_rso_puuid("<routing>", "<rso_puuid>");
// declaration
Json::Value
```

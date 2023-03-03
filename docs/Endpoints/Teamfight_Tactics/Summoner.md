### TFT-SUMMONER-V1

View [Riot's](https://developer.riotgames.com/apis#tft-summoner-v1) documentation of TFT-SUMMONER-V1


**by-account**
```cpp
// function call
Json::Value response = client_obj.Tft_Summoner.by_account("<routing>", "<account-id>");
// declaration
Json::Value by_account(std::string, std::string);
```
**by-name**
```cpp
// function call
Json::Value response = client_obj.Tft_Summoner.by_name("<routing>", "<summoner-name>");
// declaration
Json::Value by_name(std::string, std::string);
```
**by-puuid**
```cpp
// function call
Json::Value response = client_obj.Tft_Summoner.by_puuid("<routing>", "<puuid>");
// declaration
Json::Value by_puuid(std::string, std::string);
```
**by-summoner-id**
```cpp
// function call
Json::Value response = client_obj.Tft_Summoner.by_summoner_id("<routing>", "<summoner-id>");
// declaration
Json::Value by_summoner_id(std::string, std::string);
```

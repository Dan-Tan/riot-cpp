### TFT-SUMMONER-V1

View [Riot's](https://developer.riotgames.com/apis#tft-summoner-v1) documentation of TFT-SUMMONER-V1


**by-account**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Tft_Summoner.by_account("<routing>", "<account-id>");
// declaration
std::unique_ptr<std::vector<char>> by_account(std::string, std::string);
```
**by-name**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Tft_Summoner.by_name("<routing>", "<summoner-name>");
// declaration
std::unique_ptr<std::vector<char>> by_name(std::string, std::string);
```
**by-puuid**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Tft_Summoner.by_puuid("<routing>", "<puuid>");
// declaration
std::unique_ptr<std::vector<char>> by_puuid(std::string, std::string);
```
**by-summoner-id**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Tft_Summoner.by_summoner_id("<routing>", "<summoner-id>");
// declaration
std::unique_ptr<std::vector<char>> by_summoner_id(std::string, std::string);
```

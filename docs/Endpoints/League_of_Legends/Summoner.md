### SUMMONER-V4

View [Riot's](https://developer.riotgames.com/apis#summoner-v4) documentation of SUMMONER-V4.


**by-account-id**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Summoner.by_account_id("<routing>", "<account-id>")
// declaration
std::unique_ptr<std::vector<char>> by_account_id(std::string, std::string);
```
**by-name**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Summoner.by_name("<routing>", "<name>");
// declaration
std::unique_ptr<std::vector<char>> by_name(std::string, std::string);
```
**by-puuid**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Summoner.by_puuid("<routing>", "<puuid>");
// declaration
std::unique_ptr<std::vector<char>> by_puuid(std::string, std::string);
```
**by-summoner-id**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Summoner.by_summoner_id("<routing>", "<summoner-id>");
// declaration
std::unique_ptr<std::vector<char>> by_summoner_id(std::string, std::string);
```
**by-rso-puuid**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Summoner.by_rso_puuid("<routing>", "<rso_puuid>");
// declaration
std::unique_ptr<std::vector<char>> 
```

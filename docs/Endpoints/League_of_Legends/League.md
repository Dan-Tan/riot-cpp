### LEAGUE-V4

View [Riot's](https://developer.riotgames.com/apis#league-v4) documentation of LEAGUE-V4.

**challenger**
```cpp
// function call
Json::Value response = client_obj.League.challenger("<routing>", "<queue>");
// declaration
Json::Value challenger(std::string, std::string);
```
**grandmaster**
```cpp
// function call
Json::Value response = client_obj.League.grandmaster("<routing>", "<queue>");
// declaration
Json::Value grandmaster(std::string, std::string);
```
**master**
```cpp
// function call
Json::Value response = client_obj.League.master("<routing>", "<queue>");
// declaration
Json::Value master(std::string, std::string);
```
**by-summoner-id**
```cpp
// function call
Json::Value response = client_obj.League.by_summoner_id("<routing>", "<summoner-id>");
// declaration
Json::Value by_summoner_id("<routing>", "<summoner-id>");
```
**league-id**
```cpp
// function call
Json::Value response = client_obj.League.by_league_id("<routing>", "<league-id>");
// declaration
Json::Value by_league_id(std::string, std::string)
```
**specfic-league**
```cpp
// function call
Json::Value response = client_obj.League.specific_league("<routing>", "<queue>", "<tier>", "<division>", {"page", <page>});
// declaration
Json::Value specific_league(std::string, std::string, std::string, std::string, std::pair<std::string, int>);
```

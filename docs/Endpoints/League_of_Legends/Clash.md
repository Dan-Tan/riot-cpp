### CLASH-V1

View [Riot's](https://developer.riotgames.com/apis#clash-v1) documentation of CLASH-V1.

**by-summoner-id**
```cpp
// function call
Json::Value response = client_obj.Clash.by_summoner_id("<routing>", "<summoner-id>")
// declaration
Json::Value by_summoner_id(std::string, std::string);
```
**by-team**
```cpp
// function call
Json::Value response = client_obj.Clash.by_team("<routing>", "<team>");
// declaration
Json::Value by_team(std::string, std::string);
```
**tournament-by-team**
```cpp
// function call
Json::Value response = client_obj.Clash.tournament_by_team("<routing>", "<team>")
// declaration
Json::Value tournament_by_team(std::string, std::string)
```
**by-tournament**
```cpp
// function call
Json::Value response = client_obj.Clash.by_tournament("<routing>", "<tournament-id>")
// declaration
Json::Value by_tournament(std::string, std::string);
```

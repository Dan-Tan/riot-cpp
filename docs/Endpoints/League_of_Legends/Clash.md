### CLASH-V1

View [Riot's](https://developer.riotgames.com/apis#clash-v1) documentation of CLASH-V1.

**by-summoner-id**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Clash.by_summoner_id("<routing>", "<summoner-id>")
// declaration
std::unique_ptr<std::vector<char>> by_summoner_id(std::string, std::string);
```
**by-team**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Clash.by_team("<routing>", "<team>");
// declaration
std::unique_ptr<std::vector<char>> by_team(std::string, std::string);
```
**tournament-by-team**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Clash.tournament_by_team("<routing>", "<team>")
// declaration
std::unique_ptr<std::vector<char>> tournament_by_team(std::string, std::string)
```
**by-tournament**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Clash.by_tournament("<routing>", "<tournament-id>")
// declaration
std::unique_ptr<std::vector<char>> by_tournament(std::string, std::string);
```

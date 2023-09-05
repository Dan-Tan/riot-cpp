### LEAGUE-V4

View [Riot's](https://developer.riotgames.com/apis#league-v4) documentation of LEAGUE-V4.

**challenger**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.League.challenger("<routing>", "<queue>");
// declaration
std::unique_ptr<std::vector<char>> challenger(std::string, std::string);
```
**grandmaster**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.League.grandmaster("<routing>", "<queue>");
// declaration
std::unique_ptr<std::vector<char>> grandmaster(std::string, std::string);
```
**master**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.League.master("<routing>", "<queue>");
// declaration
std::unique_ptr<std::vector<char>> master(std::string, std::string);
```
**by-summoner-id**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.League.by_summoner_id("<routing>", "<summoner-id>");
// declaration
std::unique_ptr<std::vector<char>> by_summoner_id("<routing>", "<summoner-id>");
```
**league-id**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.League.by_league_id("<routing>", "<league-id>");
// declaration
std::unique_ptr<std::vector<char>> by_league_id(std::string, std::string)
```
**specfic-league**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.League.specific_league("<routing>", "<queue>", "<tier>", "<division>", {"page", <page>});
// declaration
std::unique_ptr<std::vector<char>> specific_league(std::string, std::string, std::string, std::string, std::pair<std::string, int>);
```

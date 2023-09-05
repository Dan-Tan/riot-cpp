### SPECTATOR-V4

View [Riot's](https://developer.riotgames.com/apis#spectator-v4) documentation of SPECTATOR-V4

**by-summoner-id**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Spectator.by_summoner_id("<routing>", "<summoner-id>");
// declaration
std::unique_ptr<std::vector<char>> by_summoner_id(std::string, std::string);
```
**featured-games**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Spectator.featured_games("<routing>");
// declaration
std::unique_ptr<std::vector<char>> featured_games(std::string);
```

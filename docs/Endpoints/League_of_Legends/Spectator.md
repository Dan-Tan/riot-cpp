### SPECTATOR-V4

View [Riot's](https://developer.riotgames.com/apis#spectator-v4) documentation of SPECTATOR-V4

**by-summoner-id**
```cpp
// function call
Json::Value response = client_obj.Spectator.by_summoner_id("<routing>", "<summoner-id>");
// declaration
Json::Value by_summoner_id(std::string, std::string);
```
**featured-games**
```cpp
// function call
Json::Value response = client_obj.Spectator.featured_games("<routing>");
// declaration
Json::Value featured_games(std::string);
```

### CHAMPION-MASTERY-V4

View [Riot's](https://developer.riotgames.com/apis#champion-mastery-v4) documentation CHAMPION-MASTERY-V4.

**by-summoner-id**
```cpp
// functional call
Json::Value response = client_obj.Champion_Mastery.by_summoner_id("<routing>", "<summoner-id>");
// declaration
Json::Value by_summoner_id(std::string, std::string);
```
**by-summoner-by-champion**
```cpp
// functional call
Json::Value response = client_obj.Champion_Mastery.by_summoner_by_champion("<routing>", "<summoner-id>", <champion-id>);
// declaration
Json::Value by_summoner_by_champion(std::string, std::string, int);
```
**by-summoner-top**
```cpp
// functional call
Json::Value response = client_obj.Champion_Mastery.by_summoner_top("<routing>", "<summoner-id>", {"count", <count>});
// declaration
Json::Value by_summoner_top(std::string, std::string, std::pair<std::string>, int);
```
**scores-by-summoner**
```cpp
// functional call
Json::Value response = client_obj.Champion_Mastery.scores_by_summoner("<routing>", "<summoner-id>")
// declaration
Json::Value scores_by_summoner(std::string, std::string);
```
```

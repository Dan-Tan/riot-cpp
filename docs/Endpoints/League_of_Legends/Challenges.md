### LOL-CHALLENGES-V1

View [Riot's](https://developer.riotgames.com/apis#lol-challenges-v1) documentation of LOL-CHALLENGES-V1.

**config**
```cpp
// function call
Json::Value response = client_obj.Lol_Challenges.config("<routing>");
// declaration
Json::Value config(std::string);
```
**percentiles**
```cpp
// function call
Json::Value response = client_obj.Lol_Challenges.percentiles("<routing>");
// declaration
Json::Value percentiles(std::string);
```
**challenge-config**
```cpp
// function call
Json::Value response = client_obj.Lol_Challenges.challenge_config("<routing>", <challenge_id>);
// declaration
Json::Value config(std::string, int);
```
**challenge-leaderboard**
```cpp
// function call
Json::Value response = client_obj.Lol_Challenges.challenge_leaderboard("<routing>", <challenge_id>, "<level>", {"limit", <limit>});
// declaration
Json::Value challenge_leaderboard(std::string, int, std::string, std::pair<std::string, int>);
```
**challenge-percentiles**
```cpp
Json::Value client_obj.Lol_Challenges.challenge_percentiles("<routing>", <challenge_id>) 
// declaration
Json::Value challenge_percentiles(std::string, int);
```
**by-puuid**
```cpp
// function call
Json::Value response = client_obj.Lol_Challenges.by_puuid("<routing>", "<puuid>");
// declaration
Json::Value config(std::string, std::string);
```

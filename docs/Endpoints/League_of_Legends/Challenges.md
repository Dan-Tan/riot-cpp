### LOL-CHALLENGES-V1

View [Riot's](https://developer.riotgames.com/apis#lol-challenges-v1) documentation of LOL-CHALLENGES-V1.

**config**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Lol_Challenges.config("<routing>");
// declaration
std::unique_ptr<std::vector<char>> config(std::string);
```
**percentiles**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Lol_Challenges.percentiles("<routing>");
// declaration
std::unique_ptr<std::vector<char>> percentiles(std::string);
```
**challenge-config**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Lol_Challenges.challenge_config("<routing>", <challenge_id>);
// declaration
std::unique_ptr<std::vector<char>> config(std::string, int);
```
**challenge-leaderboard**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Lol_Challenges.challenge_leaderboard("<routing>", <challenge_id>, "<level>", {"limit", <limit>});
// declaration
std::unique_ptr<std::vector<char>> challenge_leaderboard(std::string, int, std::string, std::pair<std::string, int>);
```
**challenge-percentiles**
```cpp
std::unique_ptr<std::vector<char>> client_obj.Lol_Challenges.challenge_percentiles("<routing>", <challenge_id>) 
// declaration
std::unique_ptr<std::vector<char>> challenge_percentiles(std::string, int);
```
**by-puuid**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Lol_Challenges.by_puuid("<routing>", "<puuid>");
// declaration
std::unique_ptr<std::vector<char>> config(std::string, std::string);
```

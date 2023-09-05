### TFT-LEAGUE-V1

View [Riot's](https://developer.riotgames.com/apis#tft-league-v1) documentation of TFT-LEAGUE-V1


**challenger**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Tft_League.challenger("<routing>");
// declaration
std::unique_ptr<std::vector<char>> challenger(std::string);
```
**grandmaster**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Tft_League.grandmaster("<routing>");
// declaration
std::unique_ptr<std::vector<char>> grandmaster(std::string);
```
**master**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Tft_League.master("<routing>");
// declaration
std::unique_ptr<std::vector<char>> master(std::string);
```
**by-summoner-id**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Tft_League.by_summoner_id("<routing>", "<summoner-id>");
// declaration
std::unique_ptr<std::vector<char>> by_summoner_id(std::string, std::string);
```
**by-league-id**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Tft_League.by_league_id("<routing>", "<league-id>");
// declaration
std::unique_ptr<std::vector<char>> by_league_id(std::string, std::string);
```
**queue-top**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Tft_League.queue_top("<routing>", "<queue>");
// declaration
std::unique_ptr<std::vector<char>> queue_top(std::string, std::string);
```
**by-tier-division**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Tft_League.by_tier_division("<routing>", "<tier>", "<division>", {"count", <count>});
// declaration
std::unique_ptr<std::vector<char>> by_tier_division(std::string, std::string, std::string, std::pair<std::string, int>);
```

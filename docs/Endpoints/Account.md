### ACCOUNT-V1

View [Riot's](https://developer.riotgames.com/apis#account-v1) documentation of ACCOUNT-V1

**by-puuid**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Account.by_puuid("<routing>", "<puuid>");
// declaration
std::unique_ptr<std::vector<char>> 
```
**by-riot-id**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Account.by_riot_id("<routing>", "<gameName>", "<tagline>");
// declaration
std::unique_ptr<std::vector<char>> by_riot_id(std::string, std::string, std::string);
```
**by-game**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Account.by_game("<routing>", "<game>", "<puuid>");
// declaration
std::unique_ptr<std::vector<char>> by_game(std::string, std::string, std::string);
```

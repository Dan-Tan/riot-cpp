### TFT-MATCH-V5

View [Riot's](https://developer.riotgames.com/apis#tft-match-v1) documentation of TFT-MATCH-V5


**by-puuid**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Tft_Match.by_puuid("<routing>", "<routing>", {"<optional-key>", <optional-arg>}, ...);
// declaration
std::unique_ptr<std::vector<char>> by_puuid(std::string, std::string, std::pair<std::string, int>, ...);
```
**by-match**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Tft_Match.by_match("<routing>", "<match-id>");
// declaration
std::unique_ptr<std::vector<char>> by_match(std::string, std::string);
```

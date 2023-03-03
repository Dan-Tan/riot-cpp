### TFT-MATCH-V5

View [Riot's](https://developer.riotgames.com/apis#tft-match-v1) documentation of TFT-MATCH-V5


**by-puuid**
```cpp
// function call
Json::Value response = client_obj.Tft_Match.by_puuid("<routing>", "<routing>", {"<optional-key>", <optional-arg>}, ...);
// declaration
Json::Value by_puuid(std::string, std::string, std::pair<std::string, int>, ...);
```
**by-match**
```cpp
// function call
Json::Value response = client_obj.Tft_Match.by_match("<routing>", "<match-id>");
// declaration
Json::Value by_match(std::string, std::string);
```

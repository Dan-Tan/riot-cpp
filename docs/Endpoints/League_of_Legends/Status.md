### LOL-STATUS

View Riot's documentation of LOL-STATUS below,

[**v3**](https://developer.riotgames.com/apis#lol-status-v3)

*Warning: this endpoint is depricated.*

```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Lol_Status.v3("<routing>");
// declaration
std::unique_ptr<std::vector<char>> v3(std::string);
```

[**v4**](https://developer.riotgames.com/apis#lol-status-v4)
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Lol_Status.v4("<routing>");
// declaration
std::unique_ptr<std::vector<char>> v4(std::string);
```

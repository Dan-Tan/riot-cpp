### VAL-CONTENT-V1

View [Riot's](https://developer.riotgames.com/apis#val-content-v1) documentation of VAL-CONTENT-V1

**content**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Val_Content.content("<routing>", {"locale", "<locale>"});
// declaration
std::unique_ptr<std::vector<char>> content(std::string, std::pair<std::string, std::string>);
```

### VAL-MATCH-V1


View [Riot's](https://developer.riotgames.com/apis#val-match-v1) documentation of VAL-MATCH-V1


**by-match**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Val_Match.by_match("<routing>", "<match-id>");
// declaration
std::unique_ptr<std::vector<char>> by_match(std::string, std::string);
```
**by-puuid**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Val_Match.by_puuid("<routing>", "<puuid>");
// declaration
std::unique_ptr<std::vector<char>> by_puuid(std::string, std::string);
```
**by-queue**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Val_Match.by_queue("<routing>", "<queue>");
// declaration
std::unique_ptr<std::vector<char>> by_queue(std::string, std::string);
```

### VAL-RANKED-V1

View [Riot's](https://developer.riotgames.com/apis#val-ranked-v1) documentation of VAL-RANKED-V1

**by-act**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Val_Ranked.by_act("<routing>", "<act>", {"locale", "<locale>"});
// declaration
std::unique_ptr<std::vector<char>> by_act(std::string, std::string, std::pair<std::string, std::string>);
```

### VAL-STATUS-V1

View [Riot's](https://developer.riotgames.com/apis#val-status-v1) documentation of VAL-STATUS-V1

**platform_data**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Val_Status.platform_data("<routing>");
// declaration
std::unique_ptr<std::vector<char>> platform_data(std::string);
```

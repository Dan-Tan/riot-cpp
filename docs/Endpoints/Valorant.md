### VAL-CONTENT-V1

View [Riot's](https://developer.riotgames.com/apis#val-content-v1) documentation of VAL-CONTENT-V1

**content**
```cpp
// function call
Json::Value response = client_obj.Val_Content.content("<routing>", {"locale", "<locale>"});
// declaration
Json::Value content(std::string, std::pair<std::string, std::string>);
```

### VAL-MATCH-V1


View [Riot's](https://developer.riotgames.com/apis#val-match-v1) documentation of VAL-MATCH-V1


**by-match**
```cpp
// function call
Json::Value response = client_obj.Val_Match.by_match("<routing>", "<match-id>");
// declaration
Json::Value by_match(std::string, std::string);
```
**by-puuid**
```cpp
// function call
Json::Value response = client_obj.Val_Match.by_puuid("<routing>", "<puuid>");
// declaration
Json::Value by_puuid(std::string, std::string);
```
**by-queue**
```cpp
// function call
Json::Value response = client_obj.Val_Match.by_queue("<routing>", "<queue>");
// declaration
Json::Value by_queue(std::string, std::string);
```

### VAL-RANKED-V1

View [Riot's](https://developer.riotgames.com/apis#val-ranked-v1) documentation of VAL-RANKED-V1

**by-act**
```cpp
// function call
Json::Value response = client_obj.Val_Ranked.by_act("<routing>", "<act>", {"locale", "<locale>"});
// declaration
Json::Value by_act(std::string, std::string, std::pair<std::string, std::string>);
```

### VAL-STATUS-V1

View [Riot's](https://developer.riotgames.com/apis#val-status-v1) documentation of VAL-STATUS-V1

**platform_data**
```cpp
// function call
Json::Value response = client_obj.Val_Status.platform_data("<routing>");
// declaration
Json::Value platform_data(std::string);
```

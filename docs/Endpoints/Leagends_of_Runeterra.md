### LOR-MATCH-V1

View [Riot's](https://developer.riotgames.com/apis#lor-match-v1) documentation of LOR-MATCH-V1

**by-puuid**
```cpp
// function call
Json::Value response = client_obj.Lor_Match.by_puuid("<routing>", "<puuid>");
// declaration
Json::Value by_puuid(std::string, std::string);
```
**by-match**
```cpp
// function call
Json::Value response = client_obj.Lor_Match.by_match("<routing>", "<match-id");
// declaration
Json::Value by_match(std::string, std::string);
```
### LOR-RANKED-V1

View [Riot's](https://developer.riotgames.com/apis#lor-ranked-v1) documentation of LOR-RANKED-V1

**leaderboards**
```cpp
// function call
Json::Value response = client_obj.Lor_Ranked.leaderboards("<routing>");
// declaration
Json::Value leaderboards(std::string);
```

### LOR-STATUS

View [Riot's](https://developer.riotgames.com/apis#lor-status-v1) documentation of LOR-STATUS-V1

**v1**
```cpp
// function call
Json::Value response = client_obj.Lor_Status.v1("<routing>");
// declaration
Json::Value v1(std::string);

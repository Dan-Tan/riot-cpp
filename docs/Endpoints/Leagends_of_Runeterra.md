### LOR-MATCH-V1

View [Riot's](https://developer.riotgames.com/apis#lor-match-v1) documentation of LOR-MATCH-V1

**by-puuid**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Lor_Match.by_puuid("<routing>", "<puuid>");
// declaration
std::unique_ptr<std::vector<char>> by_puuid(std::string, std::string);
```
**by-match**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Lor_Match.by_match("<routing>", "<match-id");
// declaration
std::unique_ptr<std::vector<char>> by_match(std::string, std::string);
```
### LOR-RANKED-V1

View [Riot's](https://developer.riotgames.com/apis#lor-ranked-v1) documentation of LOR-RANKED-V1

**leaderboards**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Lor_Ranked.leaderboards("<routing>");
// declaration
std::unique_ptr<std::vector<char>> leaderboards(std::string);
```

### LOR-STATUS

View [Riot's](https://developer.riotgames.com/apis#lor-status-v1) documentation of LOR-STATUS-V1

**v1**
```cpp
// function call
std::unique_ptr<std::vector<char>> response = client_obj.Lor_Status.v1("<routing>");
// declaration
std::unique_ptr<std::vector<char>> v1(std::string);

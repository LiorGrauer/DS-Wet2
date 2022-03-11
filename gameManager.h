#ifndef _GAME_MANAGER_H
#define _GAME_MANAGER_H

#include "./library2.h"
#include "./hashTable.h"
#include "./unionFind.h"
#include "./player.h"

const int START_LEVEL = 0;

class GameManager {
    private:
    int k;
    int scale;
    std::shared_ptr<Tree> total_players;
    std::shared_ptr<HashTable> players_ht;
    std::shared_ptr<UnionFind> groups_uf;

    public:
    GameManager(int k, int scale);
    ~GameManager() = default;
    StatusType mergeGroups(int GroupID1, int GroupID2);
    StatusType addPlayer(int PlayerID, int GroupID, int score);
    StatusType removePlayer(int PlayerID);
    StatusType increasePlayerIDLevel(int PlayerID, int LevelIncrease);
    StatusType changePlayerIDScore(int PlayerID, int NewScore);
    StatusType getPercentOfPlayersWithScoreInBounds (int GroupID, int score, int
    lowerLevel, int higherLevel, double * players);
    StatusType averageHighestPlayerLevelByGroup(int GroupID, int m, double * avgLevel);
    StatusType getPlayersBound(int GroupID, int score, int m, int * LowerBoundPlayers,
    int * HigherBoundPlayers);
    void quit();
};

#endif /* _GAME_MANAGER_H */
#include"./library2.h"
#include"./gameManager.h"

void* Init(int k, int scale) {
    if(k<=0 || scale > 200 || scale <=0)
        return nullptr;
    try{
        GameManager *DS = new GameManager(k,scale);
        return (void*)DS;
    }
    catch(const std::bad_alloc& ba){
        return nullptr;
    }
}

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2){
    if(DS == nullptr)
        return INVALID_INPUT;
    return ((GameManager*)DS)->mergeGroups(GroupID1, GroupID2);
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score){
    if(DS == nullptr)
        return INVALID_INPUT;
    return ((GameManager*)DS)->addPlayer(PlayerID, GroupID, score);
}

StatusType RemovePlayer(void *DS, int PlayerID){
    if(DS == nullptr)
        return INVALID_INPUT;
    return ((GameManager*)DS)->removePlayer(PlayerID);
}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease){
    if(DS == nullptr)
        return INVALID_INPUT;
    return ((GameManager*)DS)->increasePlayerIDLevel(PlayerID, LevelIncrease);
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore){
    if(DS == nullptr)
        return INVALID_INPUT;
    return ((GameManager*)DS)->changePlayerIDScore(PlayerID, NewScore);
}

StatusType GetPercentOfPlayersWithScoreInBounds (void *DS, int GroupID, int score, int
lowerLevel, int higherLevel, double * players){
    if(DS == nullptr)
        return INVALID_INPUT;
    return ((GameManager*)DS)->getPercentOfPlayersWithScoreInBounds(GroupID,
    score, lowerLevel, higherLevel, players);
}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * avgLevel){
    if(DS == nullptr)
        return INVALID_INPUT;
    return ((GameManager*)DS)->averageHighestPlayerLevelByGroup(GroupID, m, avgLevel);
}

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m, int * LowerBoundPlayers,
int * HigherBoundPlayers){
    if(DS == nullptr)
        return INVALID_INPUT;
    return ((GameManager*)DS)->getPlayersBound(GroupID, score, m, LowerBoundPlayers, HigherBoundPlayers);
}

void Quit(void **DS){
    if(DS == nullptr || *DS == nullptr)
        return;
    ((GameManager*)*DS)->quit();
    delete (GameManager*)*DS;
    *DS = nullptr;
}
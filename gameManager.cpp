#include "./gameManager.h"

GameManager::GameManager(int k, int scale) : k(k), scale(scale), total_players(new Tree()),
                        players_ht(new HashTable()), groups_uf(new UnionFind(k)) {}
                    
StatusType GameManager::mergeGroups(int GroupID1, int GroupID2){
    if(GroupID1 <= 0 || GroupID2 <= 0 || GroupID1 > k || GroupID2 > k)
        return INVALID_INPUT;
    try{
        groups_uf->unionSets(GroupID1,GroupID2);
        return SUCCESS;
    }
    catch(const std::bad_alloc& ba){
        return ALLOCATION_ERROR;
    }
}

StatusType GameManager::addPlayer(int PlayerID, int GroupID, int score){
    if(GroupID <= 0 || GroupID > k || score <= 0 || score > scale || PlayerID <= 0)
        return INVALID_INPUT;
    try{ 
        if(players_ht->find(PlayerID))
            return FAILURE;
        else {
            players_ht->insert(new Player(PlayerID, score, GroupID));
            total_players->addPlayer(START_LEVEL, score);
            groups_uf->tree(groups_uf->find(GroupID))->addPlayer(START_LEVEL, score);
            return SUCCESS;
        }
    }
    catch(const std::bad_alloc& ba){
        return ALLOCATION_ERROR;
    }
}

StatusType GameManager::removePlayer(int PlayerID){
    if(PlayerID <= 0)
        return INVALID_INPUT;
    try{ 
        Player* player = players_ht->find(PlayerID);
        if(!player)
            return FAILURE;
        else {
            total_players->removePlayer(player->getLevel(), player->getScore());
            groups_uf->tree(groups_uf->find(player->getGroup()))->removePlayer(player->getLevel(), player->getScore());
            players_ht->remove(PlayerID);
            return SUCCESS;
        }
    }
    catch(const std::bad_alloc& ba){
        return ALLOCATION_ERROR;
    }
}

StatusType GameManager::increasePlayerIDLevel(int PlayerID, int LevelIncrease){
    if(PlayerID <= 0 || LevelIncrease <= 0)
        return INVALID_INPUT;
    try{ 
        Player* player = players_ht->find(PlayerID);
        if(!player)
            return FAILURE;
        else {
            total_players->removePlayer(player->getLevel(), player->getScore());
            groups_uf->tree(groups_uf->find(player->getGroup()))->removePlayer(player->getLevel(),
                            player->getScore());
            player->increaseLevel(LevelIncrease);
            total_players->addPlayer(player->getLevel(), player->getScore());
            groups_uf->tree(groups_uf->find(player->getGroup()))->addPlayer(player->getLevel(),
                            player->getScore());
            return SUCCESS;
        }
    }
    catch(const std::bad_alloc& ba){
        return ALLOCATION_ERROR;
    }
}

StatusType GameManager::changePlayerIDScore(int PlayerID, int NewScore){
    if(PlayerID <= 0 || NewScore <= 0 || NewScore > scale)
        return INVALID_INPUT;
    try{
        Player* player = players_ht->find(PlayerID);
        if(!player)
            return FAILURE;
        else {
            total_players->removePlayer(player->getLevel(), player->getScore());
            groups_uf->tree(groups_uf->find(player->getGroup()))->removePlayer(player->getLevel(),
                            player->getScore());
            player->updateScore(NewScore);
            total_players->addPlayer(player->getLevel(), player->getScore());
            groups_uf->tree(groups_uf->find(player->getGroup()))->addPlayer(player->getLevel(),
                            player->getScore());
            return SUCCESS;
        }
    }
    catch(const std::bad_alloc& ba){
        return ALLOCATION_ERROR;
    }
}

StatusType GameManager::getPercentOfPlayersWithScoreInBounds (int GroupID, int score, int
    lowerLevel, int higherLevel, double * players){
    if(players == nullptr || GroupID < 0 || GroupID > k)
        return INVALID_INPUT; 
    double temp;
    try{
        if(GroupID == 0){
            temp = total_players->percentOfPlayersWithScoreInBounds(score, lowerLevel, higherLevel);
            if(temp == -1)
                return FAILURE;
            *players = temp;
            return SUCCESS;
        }
        else{
            temp = groups_uf->tree(GroupID)->percentOfPlayersWithScoreInBounds(score, lowerLevel, higherLevel);
            if(temp == -1)
                return FAILURE;
            *players = temp;
            return SUCCESS;
        }
    }
    catch(const std::bad_alloc& ba){
        return ALLOCATION_ERROR;
    }
}

StatusType GameManager::averageHighestPlayerLevelByGroup(int GroupID, int m, double * avgLevel){
    if(avgLevel == nullptr || m <= 0 || GroupID < 0 || GroupID > k)
        return INVALID_INPUT;
    try{
        if(GroupID == 0){
            if (total_players->getNumOfPlayers() < m)
                return FAILURE;
            *avgLevel = total_players->getAverageHighestPlayerLevelByGroup(m);
        }
        else{
            if (groups_uf->tree(GroupID)->getNumOfPlayers() < m)
                return FAILURE;
            *avgLevel = groups_uf->tree(GroupID)->getAverageHighestPlayerLevelByGroup(m);
        }
        return SUCCESS;
    }
    catch(const std::bad_alloc& ba){
        return ALLOCATION_ERROR;
    }
}

StatusType GameManager::getPlayersBound(int GroupID, int score, int m, int * LowerBoundPlayers,
    int * HigherBoundPlayers){
    if(LowerBoundPlayers == nullptr || HigherBoundPlayers == nullptr ||  m < 0 ||
        GroupID < 0 || GroupID > k || score <= 0 || score > scale)
        return INVALID_INPUT;
    try{
        if (GroupID == 0){
            if (total_players->getNumOfPlayers() < m)
                return FAILURE;
            total_players->playersBound(score, m, LowerBoundPlayers, HigherBoundPlayers);
        }    
        else{
            if (groups_uf->tree(GroupID)->getNumOfPlayers() < m)
                return FAILURE;
            groups_uf->tree(GroupID)->playersBound(score, m, LowerBoundPlayers, HigherBoundPlayers);
        }
        return SUCCESS;
    }
    catch(const std::bad_alloc& ba){
        return ALLOCATION_ERROR;
    }
}

void GameManager::quit(){
    total_players->destroy();
    players_ht = nullptr;
    groups_uf = nullptr;
}
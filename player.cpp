#include "./player.h"

Player::Player(int id, int score,int group, int level) :
               id(id), level(level), score(score), group(group) {}                

int Player::getID() const{
    return id;
}

int Player::getLevel() const{
    return level;
}

int Player::getScore() const{
    return score;
}

int Player::getGroup() const{
    return group;
}

void Player::increaseLevel(int increase){
    level += increase;
}

void Player::updateScore(int new_score){
    score = new_score;
}

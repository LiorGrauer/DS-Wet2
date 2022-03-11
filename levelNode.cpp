#include "./levelNode.h"


LevelNode::LevelNode(): level(0), sum_levels(0), height(0) {
    for (int i=0; i<=200; i++){
        this->players[i] = 0;
    }
}

LevelNode::LevelNode(int level, int sum_levels): 
        level(level), sum_levels(sum_levels), height(0) {
    for (int i=0; i<=200; i++){
        this->players[i] = 0;
    }
}

LevelNode::LevelNode(const LevelNode& node): 
        level(node.level), sum_levels(node.sum_levels), father(node.father), height(node.height){
    for (int i=1; i<=200; i++){
        this->players[i] = node.players[i];
    }
}

const std::shared_ptr<LevelNode> LevelNode::getFather() const{
    return this->father;
}

const std::shared_ptr<LevelNode> LevelNode::getLeft() const {
    return this->left;
}

const std::shared_ptr<LevelNode> LevelNode::getRight() const {
    return this->right;
}

int LevelNode::getLevel() const{
    return this->level;
}

int LevelNode::getPlayers(int i) const{
    return this->players[i];
}

int LevelNode::getSumLevels() const{
    return this->sum_levels;
}

void LevelNode::changeLeft(std::shared_ptr<LevelNode> left){
    this->left = left;
}

void LevelNode::changeRight(std::shared_ptr<LevelNode> right){
    this->right = right;
}

void LevelNode::changeFather(std::shared_ptr<LevelNode> father){
    this->father = father;
}

void LevelNode::changeHeight(int height){
    this->height = height;
}

void LevelNode::changeLevel(int level){
    this->level = level;
}

void LevelNode::changeSumLevels(int sum_levels){
    this->sum_levels = sum_levels;
}

void LevelNode::changePlayers(int player, int i){
    this->players[i] = player;
}

void LevelNode::swapData(std::shared_ptr<LevelNode> other){
    int temp = this->level;
    this->level = other->level;
    other->level = temp;
    temp = this->sum_levels;
    this->sum_levels = other->sum_levels;
    other->sum_levels = temp;
    for (int i=0; i<=200; i++){
        temp = this->players[i];
        this->players[i] = other->players[i];
        other->players[i] = temp;
    }
}

int LevelNode::getHeight(){
    return this->height;
}

bool LevelNode::operator<(const LevelNode& other) const{
    return this->level < other.level;
}

bool LevelNode::operator<(int other) const{
    return this->level < other;
}

bool LevelNode::operator==(const LevelNode& other) const{
    return this->level == other.level;
}

bool LevelNode::operator==(int other) const{
    return this->level == other;
}

void LevelNode::removeConnections(){
    this->father = nullptr;
    this->left = nullptr;
    this->right = nullptr;
}

void LevelNode::destroy(){
    this->father = nullptr;
    this->left = nullptr;
    this->right = nullptr;
}

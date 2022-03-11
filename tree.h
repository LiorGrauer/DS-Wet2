#ifndef _TREE_H
#define _TREE_H

#include "./levelNode.h"

const int L_ROLL = 2;
const int R_ROLL = -2;
const int LR = -1;
const int RL = 1;

class Tree{
    private:
        std::shared_ptr<LevelNode> head;
        std::shared_ptr<LevelNode> zero;
        int size;
        int index; 
        void updateRanks(std::shared_ptr<LevelNode> node, std::shared_ptr<LevelNode> add); //from node to the root
        void updateRanksAfterRemove(std::shared_ptr<LevelNode> start, std::shared_ptr<LevelNode> stop, std::shared_ptr<LevelNode> remove);
        bool makeRoll (std::shared_ptr<LevelNode> node);
        void makeLR (std::shared_ptr<LevelNode> node);
        void makeLL (std::shared_ptr<LevelNode> node);
        void makeRR (std::shared_ptr<LevelNode> node);
        void makeRL (std::shared_ptr<LevelNode> node);
        void inorderTour(std::shared_ptr<LevelNode> node, std::shared_ptr<LevelNode>* destination, int counter);
        void reverseInorderTour(std::shared_ptr<LevelNode> node, std::shared_ptr<LevelNode>* destination, int counter);
        void changeRankPreOrder(std::shared_ptr<LevelNode> node);
        void updateRankPostOrder(std::shared_ptr<LevelNode> node);
        void inorderNodes(std::shared_ptr<LevelNode>* destination);
        void inorderNodesTour(std::shared_ptr<LevelNode> node, std::shared_ptr<LevelNode>* destination, int counter);
        void buildEmptyTree(int size);
        void buildNodes(std::shared_ptr<LevelNode> father, int height);
        void removeLeaves(std::shared_ptr<LevelNode> node, int num);
        void addInorder(std::shared_ptr<LevelNode> node, std::shared_ptr<LevelNode>* nodes);
        std::shared_ptr<LevelNode> getMaxLevel();
        std::shared_ptr<LevelNode> getMinLevel();
        int getRank(std::shared_ptr<LevelNode> node, int score);
        int getRankSumLevels(std::shared_ptr<LevelNode> node);
        std::shared_ptr<LevelNode> findLower(int level); //returns node <= level
        std::shared_ptr<LevelNode> select(int rank); //return node that has the m-th player

    public:
        Tree();
        Tree(const Tree& tree) = default;
        ~Tree() = default;
        int getSize() const;
        const std::shared_ptr<LevelNode> getHead();
        const std::shared_ptr<LevelNode> find(int level);
        void insert(std::shared_ptr<LevelNode> node);
        void remove(int level); 
        void inorder(std::shared_ptr<LevelNode>* destination, int num = -1);
        void reverseInorder(std::shared_ptr<LevelNode>* destination, int num = -1);
        void merge(Tree& other);
        void destroy();
        void addPlayer(int level, int score);
        void removePlayer(int level, int score);
        double percentOfPlayersWithScoreInBounds(int score, int lowerLevel, int higherLevel);
        double getAverageHighestPlayerLevelByGroup(int m);
        int getNumOfPlayers();
        void playersBound(int score, int m, int* LowerBoundPlayers, int* HigherBoundPlayers);  
};

#endif    /*_TREE_H */
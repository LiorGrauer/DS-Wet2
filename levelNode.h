#ifndef _LEVEL_NODE_H
#define _LEVEL_NODE_H

#include <memory>

class LevelNode{
    private: 
        int level;
        int players[201];
        int sum_levels;
        std::shared_ptr<LevelNode> father;
        std::shared_ptr<LevelNode> left;
        std::shared_ptr<LevelNode> right;
        int height;

    public:
        LevelNode();
        LevelNode(int level, int sum_levels); 
        LevelNode(const LevelNode& node);
        ~LevelNode() = default;
        const std::shared_ptr<LevelNode> getFather() const;
        const std::shared_ptr<LevelNode> getLeft() const;
        const std::shared_ptr<LevelNode> getRight() const;
        int getLevel() const;
        int getPlayers(int i) const;
        int getSumLevels() const;
        void changeLeft(std::shared_ptr<LevelNode> left);
        void changeRight(std::shared_ptr<LevelNode> right);
        void changeFather(std::shared_ptr<LevelNode> father);
        void changeHeight(int height);
        void changeLevel(int level);
        void changeSumLevels(int sum_levels);
        void changePlayers(int player, int i);
        void swapData(std::shared_ptr<LevelNode> other);
        int getHeight();
        bool operator<(const LevelNode& other) const; 
        bool operator<(int other) const; 
        bool operator==(const LevelNode& other) const;
        bool operator==(int other) const;  
        void removeConnections();
        void destroy();  
};

#endif    /*_LEVEL_NODE_H */

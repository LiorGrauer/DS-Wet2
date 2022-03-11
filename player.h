#ifndef _PLAYER_H
#define _PLAYER_H

#include <memory>
#include "./tree.h"

/** Player class declaration */
class Player{
    private:
        int id;
        int level;
        int score;
        int group;
        
    public:
        /** Player default C'tor */
        Player(int id, int score,int group, int level = 0);

        /** Player Assignment C'tor */
        Player& operator=(const Player& player) = default;

        /** Player Copy C'tor */
        Player(const Player& player) = default;

        /** Player default D'tor */
        ~Player() = default;

        /** returns the Player ID number */
        int getID() const;

        /** returns the Player Level number */
        int getLevel() const;

        /** returns the Player Level number */
        int getScore() const;
        
        /** returns the player's group number */
        int getGroup() const;

        /** updates the player's group number */
        void increaseLevel(int level);

        /** updates the player's group number */
        void updateScore(int score);

    };

#endif  /* _PLAYER_H */

#ifndef _HEADER_PLAYER
#define _HEADER_PLAYER

#include <namespace.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Player{
    private:
        uint8_t playerID;
        sf::Time pRemTime;
        sf::Clock pClock;
        uint8_t direction;
    public:
        Player(uint8_t ID, uint8_t dir){
            playerID = ID;
            direction = dir;
        }
        
        uint8_t getDirection();
        uint8_t getScore();
        void updateRemTime(sf::Time time);
        sf::Time getRemTime();
        uint8_t getPlayerID();

};



#endif

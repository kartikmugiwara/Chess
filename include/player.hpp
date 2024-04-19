#ifndef _HEADER_PLAYER
#define _HEADER_PLAYER

#include <namespace.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
// #include <piece.hpp>

class Piece; //Forward declaration
class Player{
    private:
        uint8_t playerID;
        sf::Time pRemTime;
        sf::Clock pClock;
        uint8_t direction;
        uint8_t score;
    public:
        Player(uint8_t ID, uint8_t dir){
            playerID = ID;
            direction = dir;
        }
        
        std::vector<Piece*> alive{}, dead{};
        sf::Vector2i deathPos;
        uint8_t getDirection();
        uint8_t getScore();
        sf::Time getRemTime();
        uint8_t getPlayerID();

        void setScore(uint8_t newScore);
        void updateRemTime(sf::Time time);


};



#endif

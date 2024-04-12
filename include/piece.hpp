#ifndef _HEADER_PIECE
#define _HEADER_PIECE

#include <namespace.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>


class Piece{
public:
    Piece(){
    }

    //game logic stuff
    uint8_t getHeartbeat();
    void updateHeartbeat(uint8_t status);
    sf::Vector2i getPos();
    void updatePos(sf::Vector2i newCord);  // THINK: should this check if it can move or not or leave it to manager. BEtter to leave it to manager

    sf::Time getRemTime();
    void updateRemTime(sf::Time time);
    void setTexture(sf::Texture&);
    sf::Sprite* getSprite();

private:

    uint8_t pulse;
    sf::Time remTime;
    sf::Clock personalClock;
    sf::Vector2i pos;
        //graphic stuff
    sf::Sprite pieceSprite;
};



#endif
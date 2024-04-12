#include <piece.hpp>

uint8_t Piece::getHeartbeat(){
    return pulse;
}

void Piece::updateHeartbeat(uint8_t status){
    pulse = status;
}

sf::Vector2i Piece::getPos(){
    return pos;
}

void Piece::updatePos(sf::Vector2i newCord){ // TODO: maybe some movePos kind of thing can be added as well.
    pos = newCord;
}

void Piece::setTexture(sf::Texture& texture){
    pieceSprite.setTexture(texture);
}

sf::Sprite* Piece::getSprite(){
    return &pieceSprite;
}

sf::Time Piece::getRemTime(){
    return remTime;
}

void Piece::updateRemTime(sf::Time time){
    remTime += time;
}

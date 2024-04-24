#include <piece.hpp>
#include <gameconfig.hpp>
#include <player.hpp>
uint16_t Piece::globalPieceID = 0;
uint8_t Piece::getHeartbeat(){
    return pulse;
}

void Piece::updateHeartbeat(uint8_t status){
    pulse = status;
}

sf::Vector2i Piece::getPos(){
    return pos;
}

void Piece::updatePos(sf::Vector2i newCord, bool set){ // TODO: maybe some movePos kind of thing can be added as well.
    if(set){
        pos = newCord;
        if(this->getPieceType() == TextureID::pieceName::King)
            this->getPlayerRef()->setKingPos(pos);
    }
    pieceSprite.setPosition((newCord.x -1)  * (PIECE_SIZE + 2*PIECE_PAD) + PIECE_PAD , (newCord.y-1) * (PIECE_SIZE + 2*PIECE_PAD) + PIECE_PAD);
}

void Piece::setTexture(sf::Texture& texture){
    pieceSprite.setTexture(texture);
}

sf::Sprite* Piece::getSprite(){
    return &pieceSprite;
}

uint16_t Piece::getPieceID(){
    return pieceID;
}

TextureID::pieceName Piece::getPieceType(){
    return pieceType;
}

uint8_t Piece::getplayerID(){
    return playerID;
}

void Piece::setPlayerID(uint8_t player){
    playerID = player; 
}

uint8_t Piece::getDirection(){
    return direction;
}

Player* Piece::getPlayerRef(){
    return playerRef;
}
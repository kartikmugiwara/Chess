#ifndef _HEADER_PIECE
#define _HEADER_PIECE

#include <namespace.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <player.hpp>

struct pieceInfo{
    TextureID::pieceName pieceType;
    sf::Vector2i piecePos; 
};


class Piece{
public:

    Piece(pieceInfo pInfo, Player* playerRef){
        globalPieceID +=1;
        pieceID = globalPieceID;
        pieceType = pInfo.pieceType;
        playerID = playerRef->getPlayerID();
        direction = playerRef->getDirection();
        updatePos(pInfo.piecePos);
    }

    ~Piece(){
        
    }
    //game logic stuff
    uint8_t getHeartbeat();
    void updateHeartbeat(uint8_t status);
    sf::Vector2i getPos();
    void updatePos(sf::Vector2i newCord, bool set = 1);  // THINK: should this check if it can move or not or leave it to manager. BEtter to leave it to manager
    uint16_t getPieceID();
    uint8_t getplayerID();
    void setPlayerID(uint8_t player);

    sf::Time getRemTime();
    void updateRemTime(sf::Time time);
    void setTexture(sf::Texture&);
    sf::Sprite* getSprite();
    TextureID::pieceName getPieceType();
    uint8_t getDirection();
    Player* getPlayerRef();
 
private:

    static uint16_t globalPieceID;
    TextureID::pieceName pieceType;
    uint8_t playerID = -1;
    uint16_t pieceID;
    uint8_t direction;
    Player* playerRef;
    
    uint8_t pulse;
    sf::Vector2i pos;
        //graphic stuff
    sf::Sprite pieceSprite;
};

// class pieceManager{

//     private:

//     public:
//         Piece* createPiece(pieceInfo pInfo);
// };

#endif
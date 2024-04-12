#ifndef __INCLUDE_STATEMACHINE__
#define __INCLUDE_STATEMACHINE__

#include <gameconfig.hpp>
#include <piece.hpp>
#include <namespace.hpp>
#include <vector>
#include <map>
class Player{
    private:
    
    public:
        union{

            struct {
                Piece Queen;
                Piece King;
                Piece Rook[2];
                Piece Knight[2];
                Piece Bishop[2];
                Piece Pawn[8];
            } pieces;
            Piece pieceArr[16];
        } pieceFormat;
        // std::map<piece::ID, std::vector<sf::Vector2i> > piecePos = {{1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'd'}};// initial pos based on some map or user option menu

        uint8_t getScore();

};

class stateManager{

    private:

        sf::Time gameTime;
        uint8_t turn;
        std::vector<Player> playerList; 

        void initPieces(std::vector<Player>::iterator playerIt, uint8_t orient, sf::Vector2i pos);
    public:
        void resetGame();

};

#endif
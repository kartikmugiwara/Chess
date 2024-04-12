#include <statemachine.hpp>


bool Player::isAlive(piece::ID name){
    return 
}

void stateManager::resetGame(){

};


void stateManager::initPieces(std::vector<Player>::iterator playerIt, uint8_t orient, sf::Vector2i pos){

    // sf::Vector2f

    // Improve this, make this more customizable. Right now I am hardcoding for initial chess game
    for(uint8_t i=0; i < 8; i++ )
    {
        playerIt->pieceFormat.pieces.Pawn[i].updatePos( pos + sf::Vector2i(i, 1));
    }

    playerIt->pieceFormat.pieces.Queen    .updatePos(pos + sf::Vector2i(3, 0));
    playerIt->pieceFormat.pieces.King     .updatePos(pos + sf::Vector2i(4, 0));
    playerIt->pieceFormat.pieces.Rook[0]  .updatePos(pos);
    playerIt->pieceFormat.pieces.Rook[1]  .updatePos(pos + sf::Vector2i(8-(0+1), 0));
    playerIt->pieceFormat.pieces.Knight[0].updatePos(pos + sf::Vector2i(1, 0));
    playerIt->pieceFormat.pieces.Knight[1].updatePos(pos + sf::Vector2i(8-(1+1), 0));
    playerIt->pieceFormat.pieces.Bishop[0].updatePos(pos + sf::Vector2i(2, 0));
    playerIt->pieceFormat.pieces.Bishop[1].updatePos(pos + sf::Vector2i(8-(2+1), 0));


    playerIt->pieceFormat.pieces.Queen    .updateHeartbeat(-1);
    playerIt->pieceFormat.pieces.King     .updateHeartbeat(-1);
    playerIt->pieceFormat.pieces.Rook[0]  .updateHeartbeat(-1);
    playerIt->pieceFormat.pieces.Rook[1]  .updateHeartbeat(-1);
    playerIt->pieceFormat.pieces.Knight[0].updateHeartbeat(-1);
    playerIt->pieceFormat.pieces.Knight[1].updateHeartbeat(-1);
    playerIt->pieceFormat.pieces.Bishop[0].updateHeartbeat(-1);
    playerIt->pieceFormat.pieces.Bishop[1].updateHeartbeat(-1);

        
};

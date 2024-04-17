#ifndef __INCLUDE_STATEMACHINE__
#define __INCLUDE_STATEMACHINE__

#include <gameconfig.hpp>
#include <piece.hpp>
#include <costume.hpp>
#include <namespace.hpp>
#include <vector>
#include <map>


std::string* vecKey(sf::Vector2i vec){// limited to 26 columncs
    static std::string key = "" ;
    key = vec.x + 64;
    uint16_t a = vec.y;
    while(a/10 > 0){
        key += static_cast<char>(48 + (a%10));
        if(a>9)a= a/10;
    }
    key += static_cast<char>(48 + (a%10));
    std::reverse(key.begin(),key.end());
    return &key;
}

// std::map<TextureID::pieceName, std::vector<sf::Vector2i> > genericMoves={
//     {TextureID::pieceName::Pawn, {sf::Vector2i(), }},
//     {TextureID::pieceName::Knight, {sf::Vector2i(), sf::Vector2i(), sf::Vector2i()}},
//     {TextureID::pieceName::Bishop, {}},
//     {TextureID::pieceName::Rook, {}},
//     {TextureID::pieceName::Queen, {}},
//     {TextureID::pieceName::King, {}},
    
// };

// dont mention pawns....8 pawns by default

const pieceInfo defaultPieceList1[]=
                                {
                                    { TextureID::pieceName::Rook, sf::Vector2i(1,1)  },
                                    { TextureID::pieceName::Knight, sf::Vector2i(2,1)  },
                                    { TextureID::pieceName::Bishop, sf::Vector2i(3,1)  },
                                    { TextureID::pieceName::Queen, sf::Vector2i(4,1)  },
                                    { TextureID::pieceName::King, sf::Vector2i(5,1)  },
                                    { TextureID::pieceName::Bishop, sf::Vector2i(6,1)  },
                                    { TextureID::pieceName::Knight, sf::Vector2i(7,1)  },
                                    { TextureID::pieceName::Rook, sf::Vector2i(8,1)  },
                                };

const std::string pieceLoadFile[]=
{
   std::string("Pawn.png"),
   std::string("Knight.png"),
   std::string("Bishop.png"),
   std::string("Rook.png"),
   std::string("Queen.png"),
   std::string("King.png")
    // [TextureID::pieceName::Pawn] = "Pawn.png",
};

class Player{
    private:
        uint8_t playerID;
        sf::Time pRemTime;
        sf::Clock pClock;
        
    public:
        Player(uint8_t ID){
            playerID = ID;
        }
        
        uint8_t getScore();
        void updateRemTime(sf::Time time);
        sf::Time getRemTime();
        uint8_t getPlayerID();

};

sf::Time Player::getRemTime(){
    return pRemTime;
}

void Player::updateRemTime(sf::Time time){
    pRemTime += time;
}

uint8_t Player::getPlayerID(){
    return playerID;
}


class stateManager{

    private:
        stateManager(const stateManager&) = delete;
        void operator=(const stateManager&) = delete;

        static stateManager* instance_ptr;
        CostumeHandler pieceCostHandler;
        
        sf::Time gameTime;
        uint8_t turn;
        std::vector<Player> playerList; 
        

        // void initPieces(std::vector<Player>::iterator playerIt, uint8_t orient, sf::Vector2i pos);

        stateManager(){
            //Load texture of all characters. This is bad for adventure game
            // for 
            std::cout << "Loaded all costumes" << std::endl;
            for(uint8_t i =0; i < TextureID::pieceName::TOTAL_PIECES; i++)
                pieceCostHandler.loadCostume(static_cast<TextureID::pieceName>(i),pieceLoadFile[i]);
        }
    public:
        std::map<std::string, Piece* > worldMap;
        std::vector<Piece*> pieceVec;
        std::vector<sf::Vector2i> possibleSquares; // TODO: this is primitive, without taking many things into account
        
        static stateManager* getInstance();
        void resetGame();
        void updateBoard(Piece* t_piece, sf::Vector2i t_pos);
        Piece* getHeldRef(sf::Vector2i);
        std::vector<sf::Vector2i>* possibleSquaresList(uint8_t t_playerID, TextureID::pieceName t_pieceType, sf::Vector2i currPos);
};


stateManager* stateManager::getInstance(){
    if(stateManager::instance_ptr == nullptr)
    {
        stateManager::instance_ptr = new stateManager();
    }

    return stateManager::instance_ptr;
}

// stateManager* stateManager::instance_ptr = nullptr;

void stateManager::resetGame(){

    Player* playerA = new Player(1);
    Player* playerB = new Player(2);
    // TODO : erase worldmap 
    worldMap.clear();
    pieceVec.clear();
    uint8_t royalOffset = 0, pawnOffset=2;
    for(uint8_t i=1; i<=8 ;i++) //defaultlist size hardcoded . //TODO: Here insert could be problematic. should check if key exists and all.
    {
        sf::Vector2i pos = defaultPieceList1[i-1].piecePos + sf::Vector2i(0,royalOffset);
        Piece* royal = new Piece( pieceInfo{.pieceType=defaultPieceList1[i-1].pieceType, .piecePos = pos}, 1);
        royal->updatePos(pos);
        worldMap.insert({*vecKey(pos), royal} );
        pos = sf::Vector2i(i, pawnOffset);
        Piece* pawn = new Piece( pieceInfo{.pieceType=TextureID::pieceName::Pawn, .piecePos = pos}, 1);
        pawn->updatePos(pos);        
        worldMap.insert({*vecKey(pos), pawn} );
        royal->setTexture(pieceCostHandler.getCostume(royal->getPieceType()));
        pawn->setTexture(pieceCostHandler.getCostume(pawn->getPieceType()));
        royal->getSprite()->setScale(SCALE_F, SCALE_F);
        pawn->getSprite()->setScale(SCALE_F, SCALE_F);

        // pieceVec.push_back(pawn);
        // pieceVec.push_back();

    }
}

void stateManager::updateBoard(Piece* t_piece, sf::Vector2i t_pos){
    for(std::map<std::string, Piece* >::iterator iter = worldMap.begin();iter!=worldMap.end();iter++)
    {
        if(iter->second->getPieceID() == t_piece->getPieceID())
        {
            worldMap.erase(iter);
            worldMap.insert({*vecKey(t_pos), t_piece});
            break;
        }
        // mainWindow.draw(*((iter->second)->getSprite()));
        // ((iter->second)->getSprite())->setScale(SCALE_F, SCALE_F);
        // std::cout << "abc" <<static_cast<int>((iter->second)->getPieceID())<< std::endl;        
    }
}

Piece* stateManager::getHeldRef(sf::Vector2i mouse){
    sf::Vector2i m;
    m.x = mouse.x / (PIECE_SIZE + 2*PIECE_PAD) + 1;
    m.y = mouse.y / (PIECE_SIZE + 2*PIECE_PAD) + 1;

    std::map<std::string, Piece* >::iterator iter = worldMap.find(*vecKey(m));
    return iter == worldMap.end() ? nullptr: iter->second ;
}

std::vector<sf::Vector2i>* stateManager::possibleSquaresList(uint8_t t_playerID, TextureID::pieceName t_pieceType, sf::Vector2i currPos ){
    //TODO: direction variable incorporation for player 2. Refactor this check afterwards
    possibleSquares.clear();
    switch(t_pieceType){
        case TextureID::pieceName::Pawn: // TODO:: enpassant and shit
        {
            if(worldMap.find(*vecKey(currPos + sf::Vector2i(0,1))) == worldMap.end()) // nothing in front
                possibleSquares.push_back(currPos + sf::Vector2i(0,1));
            if(worldMap.find(*vecKey(currPos + sf::Vector2i(-1,1))) != worldMap.end() && worldMap[*vecKey(currPos + sf::Vector2i(-1,1))]->getplayerID()!= t_playerID)
                possibleSquares.push_back(currPos + sf::Vector2i(-1,1));
            if(worldMap.find(*vecKey(currPos + sf::Vector2i(1,1))) != worldMap.end() && worldMap[*vecKey(currPos + sf::Vector2i(1,1))]->getplayerID()!= t_playerID)
                possibleSquares.push_back(currPos + sf::Vector2i(1,1));
        }
        break;    
        case TextureID::pieceName::Queen:
        {

        }
        break;
        case TextureID::pieceName::Knight:
        {
            std::vector<sf::Vector2i> moves = {sf::Vector2i(1,2), sf::Vector2i(1,-2), sf::Vector2i(2,1), sf::Vector2i(2,-1), 
                                               sf::Vector2i(-1,2), sf::Vector2i(-1,-2), sf::Vector2i(-2,1), sf::Vector2i(-2,-1)};
            for(std::vector<sf::Vector2i>::iterator iter = moves.begin();iter!=moves.end();iter++)
            {
                if((currPos.x + iter->x)>0 && (currPos.y + iter->y)>0 && (currPos.y + iter->y)<=BOARD_SIZE && (currPos.x + iter->x)<=BOARD_SIZE)
                {
                    if(worldMap.find(*vecKey(currPos + *iter))!=worldMap.end())
                    {
                        if(worldMap[*vecKey(currPos + *iter)]->getplayerID()!= t_playerID )
                            possibleSquares.push_back(currPos + *iter);
                    }
                    else
                    {
                        possibleSquares.push_back(currPos + *iter);
                    }
                }
            }

        }
        break;
        case TextureID::pieceName::Bishop:
        {

        }
        break;
        case TextureID::pieceName::Rook:
        {

        }
        break;
        case TextureID::pieceName::King:
        {

        }
        break;
        default:
        {
            std::cout << "Some unknown character has entered battlefield! " << std::endl;
        }

    }
    return &possibleSquares;
}

#endif
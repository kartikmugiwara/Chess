#ifndef __INCLUDE_STATEMACHINE__
#define __INCLUDE_STATEMACHINE__

#include <gameconfig.hpp>
#include <piece.hpp>
#include <player.hpp>
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

#define TOTAL_DIR 8
sf::Vector2i pawnMoves[] = {sf::Vector2i(0,1),
                         sf::Vector2i(1,1),
                         sf::Vector2i(1,0),
                         sf::Vector2i(1,-1),
                         sf::Vector2i(0,-1),
                         sf::Vector2i(-1,-1),
                         sf::Vector2i(-1,0),
                         sf::Vector2i(-1,1),
                        };

std::vector<sf::Vector2i> knightMoves = {sf::Vector2i(1,2), sf::Vector2i(1,-2), sf::Vector2i(2,1), sf::Vector2i(2,-1), 
                                         sf::Vector2i(-1,2), sf::Vector2i(-1,-2), sf::Vector2i(-2,1), sf::Vector2i(-2,-1)};

std::vector<sf::Vector2i> bishopMoves = {sf::Vector2i(1,1), sf::Vector2i(1,-1), sf::Vector2i(-1,-1), sf::Vector2i(-1,1)};

std::vector<sf::Vector2i> rookMoves =   {sf::Vector2i(1,0), sf::Vector2i(-1,0), sf::Vector2i(0,1), sf::Vector2i(0,-1)};

std::vector<sf::Vector2i> queenMoves =  {sf::Vector2i(1,1), sf::Vector2i(1,-1), sf::Vector2i(-1,-1), sf::Vector2i(-1,1),
                                         sf::Vector2i(1,0), sf::Vector2i(-1,0), sf::Vector2i(0,1), sf::Vector2i(0,-1)};

class stateManager{

    private:
        stateManager(const stateManager&) = delete;
        void operator=(const stateManager&) = delete;

        static stateManager* instance_ptr;
        CostumeHandler pieceCostHandler, EvilPieceCostHandler;
        
        sf::Time gameTime;
        uint8_t turn;
        std::vector<Player> playerList; 
        

        // void initPieces(std::vector<Player>::iterator playerIt, uint8_t orient, sf::Vector2i pos);

        stateManager(){
            //Load texture of all characters. This is bad for adventure game
            // for 
            std::cout << "Loaded all costumes" << std::endl;
            for(uint8_t i =0; i < TextureID::pieceName::TOTAL_PIECES; i++)
            {
                pieceCostHandler.loadCostume(static_cast<TextureID::pieceName>(i),pieceLoadFile[i]);
                EvilPieceCostHandler.loadCostume(static_cast<TextureID::pieceName>(i),"Evil" + pieceLoadFile[i]);
            }

        }
    public:
        std::map<std::string, Piece* > worldMap;
        std::vector<Piece*> pieceVec;
        std::vector<sf::Vector2i> possibleSquares; // TODO: this is primitive, without taking many things into account
        
        static stateManager* getInstance();
        void resetGame();
        void updateBoard(Piece* t_piece, sf::Vector2i t_pos);
        Piece* getHeldRef(sf::Vector2i);
        std::vector<sf::Vector2i>* possibleSquaresList(uint8_t t_playerID, TextureID::pieceName t_pieceType, sf::Vector2i currPos, Piece* heldPiece);
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

    Player* playerA = new Player(1 ,0);
    Player* playerB = new Player(2, 4);
    // TODO : erase worldmap 
    worldMap.clear();
    pieceVec.clear();
    uint8_t royalOffset = 0, pawnOffset=2;
    for(uint8_t i=1; i<=8 ;i++) //defaultlist size hardcoded . //TODO: Here insert could be problematic. should check if key exists and all.
    {
        sf::Vector2i pos = defaultPieceList1[i-1].piecePos + sf::Vector2i(0,royalOffset);
        Piece* royal = new Piece( pieceInfo{.pieceType=defaultPieceList1[i-1].pieceType, .piecePos = pos}, playerA);
        royal->updatePos(pos);
        worldMap.insert({*vecKey(pos), royal} );
        pos = sf::Vector2i(i, pawnOffset);
        Piece* pawn = new Piece( pieceInfo{.pieceType=TextureID::pieceName::Pawn, .piecePos = pos}, playerA);
        pawn->updatePos(pos);        
        worldMap.insert({*vecKey(pos), pawn} );
        royal->setTexture(pieceCostHandler.getCostume(royal->getPieceType()));
        pawn->setTexture(pieceCostHandler.getCostume(pawn->getPieceType()));
        royal->getSprite()->setScale(SCALE_F, SCALE_F);
        pawn->getSprite()->setScale(SCALE_F, SCALE_F);

        // pieceVec.push_back(pawn);
        // pieceVec.push_back();

    }

    for(uint8_t i=1; i<=8 ;i++) //defaultlist size hardcoded . //TODO: Here insert could be problematic. should check if key exists and all.
    {
        sf::Vector2i pos = defaultPieceList1[i-1].piecePos + sf::Vector2i(0,royalOffset) + sf::Vector2i(0,7);
        Piece* royal = new Piece( pieceInfo{.pieceType=defaultPieceList1[i-1].pieceType, .piecePos = pos }, playerB);
        royal->updatePos(pos);
        worldMap.insert({*vecKey(pos), royal} );
        pos = sf::Vector2i(i, pawnOffset) + sf::Vector2i(0,5); 
        Piece* pawn = new Piece( pieceInfo{.pieceType=TextureID::pieceName::Pawn, .piecePos = pos }, playerB);
        pawn->updatePos(pos);        
        worldMap.insert({*vecKey(pos), pawn} );
        royal->setTexture(EvilPieceCostHandler.getCostume(royal->getPieceType()));
        pawn->setTexture(EvilPieceCostHandler.getCostume(pawn->getPieceType()));
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
            std::map<std::string, Piece* >::iterator it = worldMap.find(*vecKey(t_pos));
            if(it != worldMap.end()) // TODO:  erasing opp piece. Add it to deleted list 
                worldMap.erase(it);
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

std::vector<sf::Vector2i>* stateManager::possibleSquaresList(uint8_t t_playerID, TextureID::pieceName t_pieceType, sf::Vector2i currPos, Piece* heldPiece ){
    //TODO: direction variable incorporation for player 2. Refactor this check afterwards
    possibleSquares.clear();
    switch(t_pieceType){
        case TextureID::pieceName::Pawn: // TODO:: enpassant and shit
        {
            if(worldMap.find(*vecKey(currPos + pawnMoves[heldPiece->getDirection()])) == worldMap.end()) // nothing in front
                possibleSquares.push_back(currPos + pawnMoves[heldPiece->getDirection()]);
            if(worldMap.find(*vecKey(currPos + pawnMoves[(heldPiece->getDirection()+ (TOTAL_DIR-1))%TOTAL_DIR])) != worldMap.end() && worldMap[*vecKey(currPos + pawnMoves[(heldPiece->getDirection()+ (TOTAL_DIR-1))%TOTAL_DIR])]->getplayerID()!= t_playerID)
                possibleSquares.push_back(currPos + pawnMoves[(heldPiece->getDirection()+ (TOTAL_DIR-1))%TOTAL_DIR]);
            if(worldMap.find(*vecKey(currPos + pawnMoves[(heldPiece->getDirection()+ 1)%TOTAL_DIR])) != worldMap.end() && worldMap[*vecKey(currPos + pawnMoves[(heldPiece->getDirection()+ 1)%TOTAL_DIR])]->getplayerID()!= t_playerID)
                possibleSquares.push_back(currPos + pawnMoves[(heldPiece->getDirection()+ 1)%TOTAL_DIR]);
        }
        break;    
        case TextureID::pieceName::Knight:
        {

            for(std::vector<sf::Vector2i>::iterator iter = knightMoves.begin();iter!=knightMoves.end();iter++)
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
        case TextureID::pieceName::Rook:
        case TextureID::pieceName::Queen:
        {
            std::vector<sf::Vector2i>* moves;
            if(t_pieceType == TextureID::pieceName::Bishop)
                moves = &bishopMoves;
            else if(t_pieceType == TextureID::pieceName::Rook)
                moves = &rookMoves;
            else if(t_pieceType == TextureID::pieceName::Queen)
                moves = &queenMoves;

            uint8_t sideFinished = 0, dist = 1;

            for(std::vector<sf::Vector2i>::iterator iter = moves->begin();iter!=moves->end();iter++)
            {
                dist = 1;
                while(true)
                {
                    int newx = dist * iter->x;
                    int newy = dist * iter->y;
                    sf::Vector2i newpos = sf::Vector2i(newx,newy);
                    if((currPos.x + newx)>0 && (currPos.y + newy)>0 && (currPos.y + newy)<=BOARD_SIZE && (currPos.x + newx)<=BOARD_SIZE)
                    {
                        if(worldMap.find(*vecKey(currPos + newpos))!=worldMap.end()) //piece exists
                        {
                            if(worldMap[*vecKey(currPos + newpos)]->getplayerID()!= t_playerID )//opponent piece
                            {
                                possibleSquares.push_back(currPos + newpos);
                            }

                            break;
                        }
                        else
                        {
                            possibleSquares.push_back(currPos + newpos);
                        }
                    }
                    else
                    {
                        break;
                    }

                    dist++;
                }
            }
        }
        break;
        case TextureID::pieceName::King:
        {
                // for checking checks on king. Should I calculate reverse like a Queen and Bishop ?? Mind Blown
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
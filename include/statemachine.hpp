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

sf::Vector2i deathPosition1 = sf::Vector2i(BOARD_SIZE*(PIECE_SIZE + 2*PIECE_PAD) + 2*PIECE_PAD, (PIECE_SIZE + 2*PIECE_PAD) + 2*PIECE_PAD);
sf::Vector2i deathPosition2 = sf::Vector2i(BOARD_SIZE*(PIECE_SIZE + 2*PIECE_PAD) + 2*PIECE_PAD, (BOARD_SIZE-1)*(PIECE_SIZE + 2*PIECE_PAD));
static bool enemySpotted, pawnKill;

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
std::vector<sf::Vector2i> pawnMoves = {sf::Vector2i(0,1),
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

struct pieceState{
    Piece* piece;
    sf::Vector2i wasPos;
    sf::Vector2i isPos;
    bool killed;
    sf::Time remTime;
    // uint16_t wasKilledAtMove;
    // std::map<std::string, Piece* >::iterator worldIter;
};

typedef struct pieceState pieceState;
class stateManager{

    private:
        stateManager(const stateManager&) = delete;
        void operator=(const stateManager&) = delete;

        static stateManager* instance_ptr;
        CostumeHandler pieceCostHandler, EvilPieceCostHandler;
        
        sf::Time gameTime;
        uint8_t turn;
        sf::Font uiFont;

        uint16_t totalMoves;
        uint16_t currMove;
        
        // void initPieces(std::vector<Player>::iterator playerIt, uint8_t orient, sf::Vector2i pos);

        stateManager()
        {
            //Load texture of all characters. This is bad for adventure game
            // for 
            std::cout << "Loaded all costumes" << std::endl;
            for(uint8_t i =0; i < TextureID::pieceName::TOTAL_PIECES; i++)
            {
                pieceCostHandler.loadCostume(static_cast<TextureID::pieceName>(i),pieceLoadFile[i]);
                EvilPieceCostHandler.loadCostume(static_cast<TextureID::pieceName>(i),"Evil" + pieceLoadFile[i]);
            }

            uiFont.loadFromFile("assets/Roboto.ttf");
            playerTurn = 0;
            lastMove = movesHist.begin();
            lastDeadMove = deadHist.begin();
            currMove = 0;
            totalMoves = 0;
            // playerList.push_back(playerA);
            // playerList.push_back(playerB);


        }
    public:
        Player* playerA;
        Player* playerB;
        uint8_t playerTurn;
        std::map<std::string, Piece* > worldMap, shadowMap;
        std::vector<Piece*> deadPiece;
        std::vector<sf::Vector2i> smPossibleSquares; // TODO: this is primitive, without taking many things into account
        std::vector<Player*> playerList; 
        std::vector<pieceState> movesHist{};
        std::vector<pieceState> deadHist{};
        std::vector<pieceState>::iterator lastMove, lastDeadMove;


        static stateManager* getInstance();
        void resetGame();
        void updateBoard(Piece* t_piece, sf::Vector2i t_pos);
        void updateHell(Player* playerRef);
        void calculateScore(Player*);
        void updateTurn(bool future = 1);
        Player* whoseTurn();
        bool underCheck(Player* playerRef);
        void stepFuture();
        void stepPast();
        void updateTotalMoves(uint16_t delta);
        void updateCurrentMove(uint16_t delta);
        uint16_t getTotalMoves();
        uint16_t getCurrentMove();

        Piece* getHeldRef(sf::Vector2i);
        std::vector<sf::Vector2i>* possibleSquaresList(uint8_t t_playerID, TextureID::pieceName t_pieceType, sf::Vector2i currPos, std::vector<sf::Vector2i>& possibleSquares, Piece* heldPiece=nullptr);
        bool underCheck(Player* playerRef, sf::Vector2i piecePos );

};

void stateManager::stepFuture(){
    if(currMove == totalMoves)
        return;
    if(currMove>=1 && movesHist.begin() != movesHist.end() && lastMove != movesHist.end()-1 )
    {
        ++lastMove;
    }
    // updateTurn(1);
    ++currMove;

    lastMove->piece->updatePos(lastMove->isPos);
    worldMap.erase(worldMap.find(*vecKey(lastMove->wasPos)));

    if(lastMove->killed)
    {
        ++lastDeadMove;
        lastDeadMove->piece->getPlayerRef()->dead.push_back(lastDeadMove->piece);
        deadPiece.push_back(lastDeadMove->piece);
        lastDeadMove->piece->getSprite()->scale(0.3, 0.3);
        lastDeadMove->piece->getPlayerRef()->alive.erase(
                            std::find(lastDeadMove->piece->getPlayerRef()->alive.begin(), 
                            lastDeadMove->piece->getPlayerRef()->alive.end(),
                            lastDeadMove->piece));
        worldMap.erase(worldMap.find(*vecKey(lastMove->isPos)));
        updateHell(lastDeadMove->piece->getPlayerRef());
    }
    worldMap.insert({*vecKey(lastMove->isPos), lastMove->piece});
    
}

void stateManager::stepPast(){
    if(currMove == 0)
        return;

    lastMove->piece->updatePos(lastMove->wasPos);
    worldMap.erase(worldMap.find(*vecKey(lastMove->isPos)));
    worldMap.insert({*vecKey(lastMove->wasPos), lastMove->piece});

    if(lastMove->killed) // revival on undo
    {
        Piece* revivedPiece = *(lastDeadMove->piece->getPlayerRef()->dead.end() - 1) ;
        lastDeadMove->piece->getPlayerRef()->dead.pop_back();
        deadPiece.pop_back(); // updateshell drawing
        revivedPiece->getSprite()->scale(1/0.3, 1/0.3);
        revivedPiece->getPlayerRef()->alive.push_back(revivedPiece);
        revivedPiece->updatePos(lastMove->isPos);
        worldMap.insert({*vecKey(lastMove->isPos), revivedPiece});
        --lastDeadMove;
    }
    
    --currMove;
    // updateTurn(0);
    if(currMove == 0)
        return;
    if(lastMove != movesHist.begin())
    {
        --lastMove;
    }


}

stateManager* stateManager::getInstance(){
    if(stateManager::instance_ptr == nullptr)
    {
        stateManager::instance_ptr = new stateManager();
    }

    return stateManager::instance_ptr;
}

void stateManager::updateTotalMoves(uint16_t delta){
    totalMoves += delta;
}

void stateManager::updateCurrentMove(uint16_t delta){
    currMove += delta;
}

uint16_t stateManager::getTotalMoves(){
    return totalMoves;
}

uint16_t stateManager::getCurrentMove(){
    return currMove;
}

// stateManager* stateManager::instance_ptr = nullptr;

void stateManager::resetGame(){

    playerA = new Player(1 ,0, uiFont);
    playerB = new Player(2, 4, uiFont);
    playerList.push_back(playerA);
    playerList.push_back(playerB);
    playerA->deathPos = deathPosition1;
    playerA->scoreText.setPosition(playerA->deathPos.x, playerA->deathPos.y + pawnMoves[playerA->getDirection()].y*5*PIECE_PAD); // score string position
    playerA->scoreText.setString("Player"+std::to_string(playerA->getPlayerID())+" : "+std::to_string(0));
    playerA->timeText.setPosition(playerA->deathPos.x + 3*PIECE_PAD, playerA->deathPos.y + pawnMoves[playerA->getDirection()].y*10*PIECE_PAD);
    playerA->scoreRect.setPosition(playerA->deathPos.x, playerA->deathPos.y + pawnMoves[playerA->getDirection()].y*10*PIECE_PAD);
    
    playerB->deathPos = deathPosition2;
    playerB->scoreText.setPosition(playerB->deathPos.x, playerB->deathPos.y + pawnMoves[playerB->getDirection()].y*10*PIECE_PAD);
    playerB->scoreText.setString("Player"+std::to_string(playerB->getPlayerID())+" : "+std::to_string(0));
    playerB->timeText.setPosition(playerB->deathPos.x + 3*PIECE_PAD, playerB->deathPos.y + pawnMoves[playerB->getDirection()].y*18*PIECE_PAD);
    playerB->scoreRect.setPosition(playerB->deathPos.x, playerB->deathPos.y + pawnMoves[playerB->getDirection()].y*18*PIECE_PAD);
    playerA->pClock.restart();
    playerB->pClock.restart();



    // TODO : erase worldmap 
    worldMap.clear();
    // pieceVec.clear();
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

//instead of shadowmap. use stack for undo
void stateManager::updateBoard(Piece* t_piece, sf::Vector2i t_pos){
    for(std::map<std::string, Piece* >::iterator iter = worldMap.begin();iter!=worldMap.end();iter++)
    {
        if(iter->second->getPieceID() == t_piece->getPieceID())
        {
            totalMoves += 1;
            currMove = totalMoves;

            worldMap.erase(iter);
            std::map<std::string, Piece* >::iterator it = worldMap.find(*vecKey(t_pos));
            if(it != worldMap.end()) // TODO:  erasing opp piece. Add it to deleted list 
            {
                Player* playerRef = it->second->getPlayerRef();
                std::vector<Piece*>::iterator killedPiece = std::find(playerRef->alive.begin(), playerRef->alive.end(), it->second);
                // worldMap.erase(it);
                // worldMap.insert({*vecKey(t_pos), t_piece});

                // if(underCheck(t_piece->getPlayerRef(), t_piece->getPlayerRef()->getKingPos()))
                // {
                //     worldMap = shadowMap;
                //     return;
                // }
                pieceState* deadMove = new pieceState{*killedPiece,
                                                     (*killedPiece)->getPos(),
                                                     sf::Vector2i(0,0),
                                                     true,
                                                    //  .wasKilledAtMove = 0,
                                                    };
                deadHist.push_back(*deadMove);
                lastDeadMove = deadHist.end() - 1;
                if(killedPiece == playerRef->alive.end() )
                    std::cout<< "Deleting player logic problem" << std::endl;
                else{
                    playerRef->dead.push_back(*killedPiece);
                    deadPiece.push_back(*killedPiece);
                    (*killedPiece)->getSprite()->scale(0.3, 0.3);
                    playerRef->alive.erase(killedPiece);
                    updateHell(playerRef);
                // for(std::vector<Piece*>::iterator i = (playerRef->alive).begin();i != (playerRef->alive).end();i++)
                //     std::cout<< "alive" << (*(*i)).getPieceID() << std::endl;

                // for(std::vector<Piece*>::iterator i = (playerRef->dead).begin();i != (playerRef->dead).end();i++)
                //     std::cout<< "dead" << (*(*i)).getPieceID() << std::endl;

                }
                    // worldMap.erase(it);
                 worldMap.erase(it);
            }
            worldMap.insert({*vecKey(t_pos), t_piece});
            break;
        }
        // mainWindow.draw(*((iter->second)->getSprite()));
        // ((iter->second)->getSprite())->setScale(SCALE_F, SCALE_F);
        // std::cout << "abc" <<static_cast<int>((iter->second)->getPieceID())<< std::endl;        
    }
}

void stateManager::updateHell(Player* playerRef){
    int16_t pawnSadasyaPosX = 0;
    int16_t royalSadasyaPosX = 0;

    for(std::vector<Piece*>::iterator i = (playerRef->dead).begin();i != (playerRef->dead).end();i++)
    {
        // (*i)->updatePos(sf::Vector2i(nayaSadasyaPos.x + (playerRef->deathPos.x), nayaSadasyaPos.y + (playerRef->deathPos.y) ));
        if((*i)->getPieceType() == TextureID::pieceName::Pawn)
        {
            pawnSadasyaPosX += 2*PIECE_PAD;
            (*i)->getSprite()->setPosition(pawnSadasyaPosX + playerRef->deathPos.x , playerRef->deathPos.y -  5*PIECE_PAD *(pawnMoves[playerRef->getDirection()].y));
        }
        else
        {
            royalSadasyaPosX += 2*PIECE_PAD;
            (*i)->getSprite()->setPosition(royalSadasyaPosX + playerRef->deathPos.x , playerRef->deathPos.y);
        }

    }
    calculateScore(playerRef);
}

void stateManager::calculateScore(Player* playerRef)
{
    uint8_t score = 0; //maxx score possible 8 + 4*3 + 2*5 + 7 + King = King + 37
    for(std::vector<Piece*>::iterator i = (playerRef->alive).begin();i != (playerRef->alive).end();i++)
    {
        // (*i)->updatePos(sf::Vector2i(nayaSadasyaPos.x + (playerRef->deathPos.x), nayaSadasyaPos.y + (playerRef->deathPos.y) ));
        switch((*i)->getPieceType()){
            case TextureID::pieceName::Pawn:
                score+=1;
            break;
            case TextureID::pieceName::Knight:
            case TextureID::pieceName::Bishop:
                score+=3;
            break;
            case TextureID::pieceName::Rook:
                score+=5;
            break;
            case TextureID::pieceName::Queen:
                score+=7;
            break;
            // case TextureID::pieceName::Pawn:

            // break;  

        }

    }
    playerRef->setScore(37 - score);
    std::string str_score = std::to_string(37-score);
    if (str_score.size()==1)
        str_score = '0' + str_score;
    playerRef->scoreText.setString("Player"+std::to_string(playerRef->getPlayerID())+" : "+ str_score);
}

void stateManager::updateTurn(bool future){
    if(future)
    playerTurn = (playerTurn + 1) % playerList.size();
    else
        playerTurn = (playerTurn + (playerList.size()-1)) % playerList.size();
    // std::cout << "playerturn " << static_cast<int>(playerTurn) << std::endl;
    if(currMove == totalMoves) 
        playerList[playerTurn]->pClock.restart();
}

Player* stateManager::whoseTurn(){
    return playerList[playerTurn];
}

// std::vector<sf::Vector2i> checkKingList;
// TODO maybe use this function inside possiblesquareslist as well.
bool stateManager::underCheck(Player* playerRef, sf::Vector2i piecePos){
    // std::vector<sf::Vector2i> tempKingList;
    sf::Vector2i newPos, newPos2;
    newPos = piecePos;
    // for(uint8_t i =0; i < pawnMoves.size(); i++){
        // newPos = currPos + pawnMoves[i];
        // if(((newPos.x)>0 && (newPos.y)>0 && (newPos.x)<=BOARD_SIZE && (newPos.y)<=BOARD_SIZE))
        // {
    bool pawnFound = false;
    for(uint8_t j =0; j < pawnMoves.size(); j++){
        newPos2 = newPos + pawnMoves[j];
        if(((newPos2.x)>0 && (newPos2.y)>0 && (newPos2.x)<=BOARD_SIZE && (newPos2.y)<=BOARD_SIZE))
        {
            if (worldMap.find(*vecKey(newPos2))!=worldMap.end())
            {
                if(worldMap[*vecKey(newPos2)]->getPieceType()==TextureID::pieceName::Pawn && worldMap[*vecKey(newPos2)]->getplayerID() != playerRef->getPlayerID()){
                    // possibleSquaresList(worldMap[*vecKey(newPos2)]->getplayerID(),TextureID::pieceName::Pawn, newPos2, worldMap[*vecKey(newPos2)]);
                    // TODO how to determine if thing is pinned
                    if((newPos == newPos2 + pawnMoves[(worldMap[*vecKey(newPos2)]->getDirection()+ (TOTAL_DIR-1))%TOTAL_DIR])
                    || (newPos == newPos2 + pawnMoves[(worldMap[*vecKey(newPos2)]->getDirection()+ 1)%TOTAL_DIR]) ){
                        pawnFound = true;
                            break;
                    }
                }
            }
        }
    }

    if(pawnFound){
        return true;
    }
    // if (worldMap.find(*vecKey(newPos))!=worldMap.end()) 
    // {
    //     if(worldMap[*vecKey(newPos)]->getplayerID() == t_playerID){// our own piece
    //         continue;
    //     }
    //     if(worldMap[*vecKey(newPos)]->getPieceType() == TextureID::pieceName::King){ //opponent king{
    //         continue;
    //     }
    // }
    
    // std::cout << "newpos0" << newPos.x << newPos.y << std::endl;
    std::vector<sf::Vector2i> tempPossibleSquares;
    possibleSquaresList(playerRef->getPlayerID(), TextureID::pieceName::Queen, newPos, tempPossibleSquares);
    if(enemySpotted)
    {
        return true;
    }
    
    possibleSquaresList(playerRef->getPlayerID(), TextureID::pieceName::Knight, newPos, tempPossibleSquares);
    if(!enemySpotted)
    {
        return false;
        // tempKingList.push_back(newPos);
    }else{
        return true;
    }
}


Piece* stateManager::getHeldRef(sf::Vector2i mouse){
    sf::Vector2i m;
    m.x = mouse.x / (PIECE_SIZE + 2*PIECE_PAD) + 1;
    m.y = mouse.y / (PIECE_SIZE + 2*PIECE_PAD) + 1;

    std::map<std::string, Piece* >::iterator iter = worldMap.find(*vecKey(m));
    return iter == worldMap.end() ? nullptr: iter->second ;
}

std::vector<sf::Vector2i>* stateManager::possibleSquaresList(uint8_t t_playerID, TextureID::pieceName t_pieceType, sf::Vector2i currPos, std::vector<sf::Vector2i>& possibleSquares, Piece* heldPiece ){
    //TODO: direction variable incorporation for player 2. Refactor this check afterwards
    // heldpiece needed only to get pawn direcction
    possibleSquares.clear();
    enemySpotted = false;
    switch(t_pieceType){
        case TextureID::pieceName::Pawn: // TODO:: enpassant and shit
        {
            if(worldMap.find(*vecKey(currPos + pawnMoves[heldPiece->getDirection()])) == worldMap.end()) // nothing in front
            {
                possibleSquares.push_back(currPos + pawnMoves[heldPiece->getDirection()]);
            }    
            if(worldMap.find(*vecKey(currPos + pawnMoves[(heldPiece->getDirection()+ (TOTAL_DIR-1))%TOTAL_DIR])) != worldMap.end() && worldMap[*vecKey(currPos + pawnMoves[(heldPiece->getDirection()+ (TOTAL_DIR-1))%TOTAL_DIR])]->getplayerID()!= t_playerID)
            {
                possibleSquares.push_back(currPos + pawnMoves[(heldPiece->getDirection()+ (TOTAL_DIR-1))%TOTAL_DIR]);
                pawnKill =true;
            }    
            if(worldMap.find(*vecKey(currPos + pawnMoves[(heldPiece->getDirection()+ 1)%TOTAL_DIR])) != worldMap.end() && worldMap[*vecKey(currPos + pawnMoves[(heldPiece->getDirection()+ 1)%TOTAL_DIR])]->getplayerID()!= t_playerID)
            {
                possibleSquares.push_back(currPos + pawnMoves[(heldPiece->getDirection()+ 1)%TOTAL_DIR]);
                pawnKill = true;
            }
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
                        {
                            possibleSquares.push_back(currPos + *iter);
                            if(worldMap[*vecKey(currPos + *iter)]->getPieceType() == TextureID::pieceName::Knight)
                            {
                                // std::cout << "knight spotted " << *vecKey(currPos + *iter) <<std::endl;
                                enemySpotted = true;
                            }
                        }
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
                                if(worldMap[*vecKey(currPos + newpos)]->getPieceType() == TextureID::pieceName::Queen
                                || worldMap[*vecKey(currPos + newpos)]->getPieceType() == TextureID::pieceName::Bishop
                                || worldMap[*vecKey(currPos + newpos)]->getPieceType() == TextureID::pieceName::Rook ){
                                    enemySpotted = true;
                                // std::cout << "piece spotted " << *vecKey(currPos + *iter) << static_cast<int>(t_pieceType) <<std::endl;
                                }
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
                // check separately for pawn because pawnmoves are fucking weird here.
                // TODO: this algo can be a lot better for pawn check
            std::vector<sf::Vector2i> tempKingList;
            sf::Vector2i newPos, newPos2;
            for(uint8_t i =0; i < pawnMoves.size(); i++){
                newPos = currPos + pawnMoves[i];
                if(((newPos.x)>0 && (newPos.y)>0 && (newPos.x)<=BOARD_SIZE && (newPos.y)<=BOARD_SIZE))
                {
                    bool pawnFound = false;
                    for(uint8_t j =0; j < pawnMoves.size(); j++){
                        newPos2 = newPos + pawnMoves[j];
                        if(((newPos2.x)>0 && (newPos2.y)>0 && (newPos2.x)<=BOARD_SIZE && (newPos2.y)<=BOARD_SIZE))
                        {
                            if (worldMap.find(*vecKey(newPos2))!=worldMap.end())
                            {
                                if(worldMap[*vecKey(newPos2)]->getPieceType()==TextureID::pieceName::Pawn && worldMap[*vecKey(newPos2)]->getplayerID() != t_playerID){
                                    // possibleSquaresList(worldMap[*vecKey(newPos2)]->getplayerID(),TextureID::pieceName::Pawn, newPos2, worldMap[*vecKey(newPos2)]);
                                    // TODO how to determine if thing is pinned
                                    if((newPos == newPos2 + pawnMoves[(worldMap[*vecKey(newPos2)]->getDirection()+ (TOTAL_DIR-1))%TOTAL_DIR])
                                    || (newPos == newPos2 + pawnMoves[(worldMap[*vecKey(newPos2)]->getDirection()+ 1)%TOTAL_DIR]) ){
                                        pawnFound = true;
                                         break;
                                    }
                                }
                            }
                        }
                    }

                    if(pawnFound){
                        continue;
                    }
                    if (worldMap.find(*vecKey(newPos))!=worldMap.end()) 
                    {
                        if(worldMap[*vecKey(newPos)]->getplayerID() == t_playerID){// our own piece
                            continue;
                        }
                        if(worldMap[*vecKey(newPos)]->getPieceType() == TextureID::pieceName::King){ //opponent king{
                            continue;
                        }
                    }
                    
                    // std::cout << "newpos0" << newPos.x << newPos.y << std::endl;
                    std::vector<sf::Vector2i> tempPossibleSquares;
                    possibleSquaresList(t_playerID, TextureID::pieceName::Queen, newPos, tempPossibleSquares);
                    if(enemySpotted)
                    {
                        continue;
                    }
                    
                    possibleSquaresList(t_playerID, TextureID::pieceName::Knight, newPos, tempPossibleSquares);
                    if(!enemySpotted)
                    {
                        tempKingList.push_back(newPos);
                    }
                    
                }
            }
            possibleSquares.clear();
            possibleSquares.assign(tempKingList.begin(), tempKingList.end());
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
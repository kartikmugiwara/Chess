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

// dont mention pawns....8 pawns by default

const pieceInfo defaultPieceList1[]=
                                {
                                    { TextureID::pieceName::Rook, sf::Vector2i(1,0)  },
                                    { TextureID::pieceName::Knight, sf::Vector2i(2,0)  },
                                    { TextureID::pieceName::Bishop, sf::Vector2i(3,0)  },
                                    { TextureID::pieceName::Queen, sf::Vector2i(4,0)  },
                                    { TextureID::pieceName::King, sf::Vector2i(5,0)  },
                                    { TextureID::pieceName::Bishop, sf::Vector2i(6,0)  },
                                    { TextureID::pieceName::Knight, sf::Vector2i(7,0)  },
                                    { TextureID::pieceName::Rook, sf::Vector2i(8,0)  },
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
        static stateManager* getInstance();
        void resetGame();
        void refreshBoard();

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
    uint8_t royalOffset = 0, pawnOffset=1;
    for(uint8_t i=1; i<=8 ;i++) //defaultlist size hardcoded . //TODO: Here insert could be problematic. should check if key exists and all.
    {
        sf::Vector2i pos = defaultPieceList1[i-1].piecePos + sf::Vector2i(0,royalOffset);
        Piece* royal = new Piece( pieceInfo{.pieceType=defaultPieceList1[i-1].pieceType, .piecePos = pos}, 1);
        worldMap.insert({*vecKey(pos), royal} );
        pos = sf::Vector2i(i, pawnOffset);
        Piece* pawn = new Piece( pieceInfo{.pieceType=TextureID::pieceName::Pawn, .piecePos = pos}, 1);
        worldMap.insert(std::pair<std::string,Piece* >(*vecKey(pos), pawn) );
        royal->setTexture(pieceCostHandler.getCostume(royal->getPieceType()));
        pawn->setTexture(pieceCostHandler.getCostume(pawn->getPieceType()));

    }
}

void stateManager::refreshBoard(){
    for(std::map<std::string, Piece* >::iterator iter = worldMap.begin();iter!=worldMap.end();iter++)
    {
        // mainWindow.draw(*((iter->second)->getSprite()));
        ((iter->second)->getSprite())->setScale(6.0, 6.0);
        // std::cout << "abc" <<static_cast<int>((iter->second)->getPieceID())<< std::endl;        
    }
}

#endif
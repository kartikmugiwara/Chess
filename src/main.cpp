// #include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string.h>
#include <assert.h>
#include <namespace.hpp>
#include <piece.hpp>
#include <costume.hpp>

#include <statemachine.hpp>



// function that taks piece, current pos and spits possible squares?
// possible square()

stateManager* stateManager::instance_ptr =nullptr;
stateManager* smInst = stateManager::getInstance();

bool justHadCheck = false;
std::vector<sf::Vector2i> tempPossibleSquares;

class movementCircle
{
    movementCircle(){
        for(uint8_t i=0; i<BOARD_SIZE; i++)
            for(uint8_t j=0; j<BOARD_SIZE; j++)
            {
                cshape[i][j].setRadius(10);
                cshape[i][j].setPointCount(30);
                cshape[i][j].setPosition((i)*(PIECE_SIZE+2*PIECE_PAD)+(PIECE_SIZE/2 ), (j*(PIECE_SIZE+2*PIECE_PAD))+(PIECE_SIZE/2 ));
                cshape[i][j].setFillColor(sf::Color(0, 0, 0, 0));
            }
    }


    private:
        movementCircle(const movementCircle&) = delete;
        void operator=(const movementCircle&) = delete;

        static movementCircle* instance_ptr;
        static sf::CircleShape cshape[BOARD_SIZE][BOARD_SIZE];

    public:
        static movementCircle* getInstance();
        static void renderAll(sf::RenderWindow&);

        void setVisible(uint8_t i, uint8_t j);
        void setInvisible(uint8_t i, uint8_t j);
};

movementCircle* movementCircle::getInstance(){
    if(movementCircle::instance_ptr == nullptr)
    {
        movementCircle::instance_ptr = new movementCircle();
    }

    return movementCircle::instance_ptr;
}

void movementCircle::setVisible(uint8_t i, uint8_t j){
    movementCircle::cshape[i-1][j-1].setFillColor(sf::Color(128, 0, 0, 80));
}


void movementCircle::setInvisible(uint8_t i, uint8_t j){
    movementCircle::cshape[i-1][j-1].setFillColor(sf::Color(128, 0, 0, 0));
}

void movementCircle::renderAll(sf::RenderWindow& window){
    movementCircle* mcInst = movementCircle::getInstance();
    for(uint8_t i=0; i<BOARD_SIZE; i++)
        for(uint8_t j=0; j<BOARD_SIZE; j++)
        {
            // mcInst->setVisible(i, j);
            window.draw(movementCircle::cshape[i][j]);
        }
}

sf::CircleShape movementCircle::cshape[BOARD_SIZE][BOARD_SIZE];
movementCircle* movementCircle::instance_ptr =nullptr;
movementCircle* mcInst = movementCircle::getInstance();


class GameHandler{
    public:
        GameHandler();
        void run();
    private:
        void render();
        void processInputs();
        void update(sf::Time);
        void handleKeyInput(sf::Keyboard::Key key, bool state );

    private:
        sf::RenderWindow mainWindow;
        sf::Clock  masterClock;
        sf::CircleShape testBall;
        sf::Texture testTexture;
        sf::Sprite player;
        char direction;
        bool isMousePressed = false;
        bool isMouseReleased = false;
        bool checkedPiece = false;
        uint8_t pieceSelectFlag=0;
        Piece* heldPiece{nullptr}, *previousHeldPiece{nullptr};
        std::vector<sf::Vector2i>* possibleSquares{nullptr};

        Piece* currentHeldPiece;
        // Piece pawn;


    // protected:
};

void GameHandler::render(){
    mainWindow.clear();

    for(std::map<std::string, Piece* >::iterator iter = (smInst->worldMap).begin();iter!=(smInst->worldMap).end();iter++)
    {
        mainWindow.draw(*((iter->second)->getSprite()));
        // std::cout << "abc" <<static_cast<int>((iter->second)->getPieceID())<< std::endl;        
        // std::cout << "abc" <<static_cast<int>((iter->second)->getPos().x)<< " " << static_cast<int>((iter->second)->getPos().y) << std::endl;        

    }

    for(std::vector<Piece*>::iterator iter = (smInst->deadPiece).begin();iter!=(smInst->deadPiece).end();iter++)
    {
        mainWindow.draw(*((*iter)->getSprite()));
        // std::cout << "abc" <<static_cast<int>((iter->second)->getPieceID())<< std::endl;        
        // std::cout << "abc" <<static_cast<int>((iter->second)->getPos().x)<< " " << static_cast<int>((iter->second)->getPos().y) << std::endl;        

    }

    for(std::vector<Player*>::iterator i = smInst->playerList.begin(); i != smInst->playerList.end();i++){
        if((*i)->getPlayerID() == smInst->whoseTurn()->getPlayerID()){
            (*i)->scoreRect.setFillColor(sf::Color(255,255,255,255));
        }else{
            (*i)->scoreRect.setFillColor(sf::Color(255,255,255,30));
        }

        // if(justHadCheck){
            mainWindow.draw((*i)->kingStatus);
        //     justHadCheck = false;
        // }
        mainWindow.draw((*i)->scoreRect);
        mainWindow.draw((*i)->scoreText);
        mainWindow.draw((*i)->timeText);

    }

    mcInst->renderAll(mainWindow);

    // std::cout << "abc" << std::endl;
    // mainWindow.draw(*pawn.getSprite());
    mainWindow.display();
}

void GameHandler::processInputs(){
    sf::Event windowEvents;
    while(mainWindow.pollEvent(windowEvents)){
        switch (windowEvents.type){
            case sf::Event::KeyPressed:
                handleKeyInput(windowEvents.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handleKeyInput(windowEvents.key.code, false);
                break;
            case sf::Event::MouseButtonPressed:
                // std::cout << "Button Down : "<< sf::Mouse::getPosition(mainWindow).x << " " << sf::Mouse::getPosition(mainWindow).y << std::endl;
                if(smInst->getTotalMoves() == smInst->getCurrentMove())
                    isMousePressed = true;
                break;
            case sf::Event::MouseButtonReleased:
                // std::cout << "Button Up : "<< sf::Mouse::getPosition(mainWindow).x << " " << sf::Mouse::getPosition(mainWindow).y << std::endl;
                if(smInst->getTotalMoves() == smInst->getCurrentMove())
                {
                    isMousePressed = false;
                    isMouseReleased = true;
                    if(checkedPiece)
                    {
                        checkedPiece = false;
                        if(pieceSelectFlag == 2 )
                        {
                            pieceSelectFlag = 0;
                            // for(std::vector<sf::Vector2i>::iterator iter = possibleSquares->begin(); iter!=possibleSquares->end();iter++ )
                            // {
                            //     std::cout<< iter->x << " " << iter->y << std::endl;
                            //     mcInst->setInvisible(iter->x, iter->y);
                            // }
                        }
                    }
                }
                break;
            case sf::Event::Closed:
                mainWindow.close();
                break;
        }
    }
}

void GameHandler::update(sf::Time deltaTime){
    // testBall.setRadius( (int)(masterClock.getElapsedTime().asMilliseconds() )% 255);
    sf::Vector2i mousePos = sf::Mouse::getPosition(mainWindow);
    testBall.setFillColor(sf::Color(0.3*masterClock.getElapsedTime().asMilliseconds(),128,233,100));
    sf::Vector2f movement(0.0f, 0.0f);
    #define DELTA 500.0f
    if(direction & 0x01)
        movement.y += -DELTA;
    if(direction & 0x02)
        movement.x += -DELTA;
    if(direction & 0x04)
        movement.y += DELTA;
    if(direction & 0x08)
        movement.x += DELTA;
        
    smInst->whoseTurn()->updateRemTime();
    // if(smInst->getTotalMoves() != smInst->getCurrentMove())
    // {
    //     sf::Time remTime = smInst->lastMove->remTime;
    //     uint16_t timeM = ((uint16_t)remTime.asSeconds())/60;
    //     uint16_t timeS = (uint16_t)(remTime.asSeconds())%60;
    //     std::string sec_str = std::to_string(timeS);
    //     if (sec_str.size()==1)
    //         sec_str = '0' + sec_str;
    //     std::string min_str = std::to_string(timeM);
    //     if (min_str.size()==1)
    //         min_str = '0' + min_str;
    //     smInst->whoseTurn()->timeText.setString(min_str + " : " + sec_str);
    // }
    testBall.move(movement * deltaTime.asSeconds());
    if(isMousePressed)
    {
        // BOUND Cursor
        mousePos = sf::Mouse::getPosition(mainWindow);

        if(!checkedPiece){
            previousHeldPiece = heldPiece;
            heldPiece = smInst->getHeldRef(mousePos);
            if(heldPiece!=nullptr && smInst->whoseTurn()->getPlayerID() != heldPiece->getplayerID()) // clicked on opponent piece
                heldPiece = nullptr;
            // smInst->whoseTurn()->getPlayerID();
            // std::cout << "heldpiece : " << static_cast<int>(heldPiece->getPieceType()) << std::endl;
            if(heldPiece!=nullptr){

                //invisble current held piece's possible squares to draw new piece's squares
                if(heldPiece != previousHeldPiece && previousHeldPiece!=nullptr )
                {
                    for(std::vector<sf::Vector2i>::iterator iter = possibleSquares->begin(); iter!=possibleSquares->end();iter++ )
                    {
                        // std::cout<< iter->x << " inv " << iter->y << std::endl;
                        mcInst->setInvisible(iter->x, iter->y);
                    }
                    pieceSelectFlag = 0;
                }
                if(previousHeldPiece != nullptr && heldPiece->getplayerID() != previousHeldPiece->getplayerID() && std::find(possibleSquares->begin(), possibleSquares->end(), heldPiece->getPos())!=possibleSquares->end())
                {
                    //kill opposite player goti
                    // std::cout << "Kill goti" << std::endl;
                    // previousHeldPiece = heldPiece;
                    heldPiece = nullptr;
                    pieceSelectFlag = 1;
                }
                else
                {
                    possibleSquares = smInst->possibleSquaresList(heldPiece->getplayerID(), heldPiece->getPieceType(), heldPiece->getPos(), smInst->smPossibleSquares, heldPiece, true);
                    for(std::vector<sf::Vector2i>::iterator iter = possibleSquares->begin(); iter!=possibleSquares->end();iter++ )
                    {
                        // std::cout<< iter->x << " vi " << iter->y << std::endl;
                        mcInst->setVisible(iter->x, iter->y);
                    }
                    
                    if(previousHeldPiece == heldPiece &&  previousHeldPiece!=nullptr && pieceSelectFlag==1)
                    {
                        pieceSelectFlag = 2;
                    }
                    if (pieceSelectFlag==0)
                    {
                        pieceSelectFlag = 1;
                    }
                }


            }
            else // clicked on some blank space. toggle selection
            {
                // previousHeldPiece = nullptr;
                pieceSelectFlag = 1;

            }
            checkedPiece = true;
        }
        if(heldPiece != nullptr) // Also write function to check valid square and single click processing as well
        {
             //note use of 0, so getpos is not updated here only sprite pos
            if( mousePos.x < 0)
                sf::Mouse::setPosition(sf::Vector2i(0,mousePos.y), mainWindow);
            if( mousePos.y < 0)
                sf::Mouse::setPosition(sf::Vector2i(mousePos.x,0), mainWindow);
            if( mousePos.x > WINDOW_BIND_Y)
                sf::Mouse::setPosition(sf::Vector2i(WINDOW_BIND_Y, mousePos.y ), mainWindow);
            if( mousePos.y > WINDOW_BIND_X)
                sf::Mouse::setPosition(sf::Vector2i(mousePos.x, WINDOW_BIND_X), mainWindow);
                
            heldPiece->updatePos(sf::Vector2i(1 + mousePos.x/(PIECE_SIZE + 2*PIECE_PAD), 1 + mousePos.y/(PIECE_SIZE + 2*PIECE_PAD)), 0);
            // heldPiece->getSprite()->setPosition(static_cast<sf::Vector2f>(mousePos)); // For smooth drag movement
        
        }
        // std::cout << "Button now : "<< mousePos.x << " " << mousePos.y << std::endl;
    }

// piece final movement happens here only, so check for checks and mate here
    if(isMouseReleased){
        // if()// TODO check if position is valid
        if(heldPiece != nullptr)
        {
            // if kept at new square or double clicked on same piece then set indicator incisible
            if(heldPiece->getPos() != sf::Vector2i(1 + mousePos.x/(PIECE_SIZE + 2*PIECE_PAD), 1 + mousePos.y/(PIECE_SIZE + 2*PIECE_PAD))
            || pieceSelectFlag==0)
            {
                for(std::vector<sf::Vector2i>::iterator iter = possibleSquares->begin(); iter!=possibleSquares->end();iter++ )
                {
                    mcInst->setInvisible(iter->x, iter->y);
                }
            }

            // update only if kept on valid square. Piece pos actual update;
            if(std::find(possibleSquares->begin(), possibleSquares->end(), sf::Vector2i(1 + mousePos.x/(PIECE_SIZE + 2*PIECE_PAD), 1 + mousePos.y/(PIECE_SIZE + 2*PIECE_PAD))) != possibleSquares->end())
            {
                pieceState* currMove = new pieceState{.piece = heldPiece,
                                                     .wasPos = heldPiece->getPos(),
                                                     .isPos = sf::Vector2i(1 + mousePos.x/(PIECE_SIZE + 2*PIECE_PAD),
                                                                           1 + mousePos.y/(PIECE_SIZE + 2*PIECE_PAD)),
                                                     .killed = false,
                                                     .remTime = smInst->whoseTurn()->getRemTime()
                                                    //  .wasKilledAtMove = 0,
                                                    };
                heldPiece->updatePos(sf::Vector2i(1 + mousePos.x/(PIECE_SIZE + 2*PIECE_PAD), 1 + mousePos.y/(PIECE_SIZE + 2*PIECE_PAD)));
                
                size_t deadLength = smInst->deadPiece.size();
                smInst->updateBoard(heldPiece, sf::Vector2i(1 + mousePos.x/(PIECE_SIZE + 2*PIECE_PAD), 1 + mousePos.y/(PIECE_SIZE + 2*PIECE_PAD)) );
                if(deadLength != smInst->deadPiece.size()){
                    currMove->killed = true;
                }

                smInst->movesHist.push_back(*currMove);
                smInst->lastMove = smInst->movesHist.end()-1;
                smInst->whoseTurn()->stat.undercheck = smInst->underCheck(smInst->whoseTurn(), smInst->whoseTurn()->getKingPos());
                if(smInst->whoseTurn()->stat.undercheck){
                    smInst->stepPast();
                    smInst->updateTotalMoves(-1);
                    smInst->movesHist.pop_back();
                    if(currMove->killed)
                    {
                        smInst->deadHist.pop_back();
                    }
                    // std::cout << "under check curr player, then piece is pinned"<< std::endl;
                }
                else
                {
                    if(justHadCheck)
                    {
                        smInst->whoseTurn()->kingStatus.setFillColor(sf::Color(CHECK_COLOR,0));
                        smInst->whoseTurn()->kingStatus.setOutlineColor(sf::Color(CHECK_OUT_COLOR,0));
                    }
                    smInst->updateTurn();
                }
                // smInst->updateTurn();
                smInst->whoseTurn()->stat.undercheck = smInst->underCheck(smInst->whoseTurn(), smInst->whoseTurn()->getKingPos());
                if(smInst->whoseTurn()->stat.undercheck){
                    // std::cout << "under check opp player"<< std::endl;
                    smInst->whoseTurn()->kingStatus.setFillColor(sf::Color(CHECK_COLOR,255));
                    smInst->whoseTurn()->kingStatus.setOutlineColor(sf::Color(CHECK_OUT_COLOR,255));
                    justHadCheck = true;
                    // TODO check for checkmate
                    // this is going to be tough mf.
                    // for(std::vector<Piece*>::iterator iter=smInst->kingAttacker.begin();iter!=smInst->kingAttacker.end();iter++)
                    //     {
                    //         std::cout << (*iter)->getPieceType() << (*iter)->getPos().x << (*iter)->getPos().y << std::endl;
                    //     }
                    //     for(std::vector<sf::Vector2i>::iterator iter=smInst->smAttackSquares.begin();iter!=smInst->smAttackSquares.end();iter++)
                    //     {
                    //         std::cout << "attack squares";
                    //         std::cout << iter->x << iter->y << std::endl;
                    //         // smInst->kingAttacker.clear();
                    //     }
                    // uint16_t possMovesSum = 0;
                    smInst->gameEnded = true;
                    for(std::vector<Piece*>::iterator iter = smInst->whoseTurn()->alive.begin(); iter!=smInst->whoseTurn()->alive.end(); iter++){
                        smInst->possibleSquaresList(smInst->whoseTurn()->getPlayerID(), (*iter)->getPieceType(), (*iter)->getPos(), tempPossibleSquares, *iter,true);
                        // std::cout << "possibleSquares size : " << tempPossibleSquares.size() << *vecKey((*iter)->getPos()) <<std::endl;
                        if(tempPossibleSquares.size() != 0)
                        {
                            smInst->gameEnded = false;
                            break;
                        }
                    }



                }else
                {
                    // if(justHadCheck)
                    // {
                    //     smInst->whoseTurn()->kingStatus.setFillColor(CHECK_COLOR);
                    //     smInst->whoseTurn()->kingStatus.setOutlineColor(sf::Color(255,255,255,0));
                    // }
                }
            }
            else{
                if(pieceSelectFlag!=0) // pakad ke galat jagah rakho to red dot delete mat karna
                    for(std::vector<sf::Vector2i>::iterator iter = possibleSquares->begin(); iter!=possibleSquares->end();iter++ )
                    {
                        mcInst->setVisible(iter->x, iter->y);
                    }
                heldPiece->updatePos(heldPiece->getPos());
                // smInst->updateTurn();


            }


        }
        else
        {
            // click to place somewhere. Actual update 
            if(pieceSelectFlag == 1 && previousHeldPiece!=nullptr)
            {
                if(std::find(possibleSquares->begin(), possibleSquares->end(), sf::Vector2i(1 + mousePos.x/(PIECE_SIZE + 2*PIECE_PAD), 1 + mousePos.y/(PIECE_SIZE + 2*PIECE_PAD))) != possibleSquares->end())
                {
                    // previousHeldPiece->updatePos(sf::Vector2i(1 + mousePos.x/(PIECE_SIZE + 2*PIECE_PAD), 1 + mousePos.y/(PIECE_SIZE + 2*PIECE_PAD)));
                    // smInst->updateBoard(previousHeldPiece, sf::Vector2i(1 + mousePos.x/(PIECE_SIZE + 2*PIECE_PAD), 1 + mousePos.y/(PIECE_SIZE + 2*PIECE_PAD)) );
                    // smInst->updateTurn();

                    pieceState* currMove = new pieceState{.piece = previousHeldPiece,
                                                     .wasPos = previousHeldPiece->getPos(),
                                                     .isPos = sf::Vector2i(1 + mousePos.x/(PIECE_SIZE + 2*PIECE_PAD),
                                                                           1 + mousePos.y/(PIECE_SIZE + 2*PIECE_PAD)),
                                                     .killed = false,
                                                     .remTime = smInst->whoseTurn()->getRemTime()
                                                    //  .wasKilledAtMove = 0,
                                                    };
                    previousHeldPiece->updatePos(sf::Vector2i(1 + mousePos.x/(PIECE_SIZE + 2*PIECE_PAD), 1 + mousePos.y/(PIECE_SIZE + 2*PIECE_PAD)));
                    
                    size_t deadLength = smInst->deadPiece.size();
                    smInst->updateBoard(previousHeldPiece, sf::Vector2i(1 + mousePos.x/(PIECE_SIZE + 2*PIECE_PAD), 1 + mousePos.y/(PIECE_SIZE + 2*PIECE_PAD)) );
                    if(deadLength != smInst->deadPiece.size()){
                        currMove->killed = true;
                    }

                    smInst->movesHist.push_back(*currMove);
                    smInst->lastMove = smInst->movesHist.end()-1;
                    smInst->whoseTurn()->stat.undercheck = smInst->underCheck(smInst->whoseTurn(), smInst->whoseTurn()->getKingPos());
                    if(smInst->whoseTurn()->stat.undercheck){
                        smInst->stepPast();
                        smInst->updateTotalMoves(-1);
                        smInst->movesHist.pop_back();
                        if(currMove->killed)
                        {
                            smInst->deadHist.pop_back();
                        }
                        // std::cout << "under check curr player, then piece is pinned"<< std::endl;
                    }
                    else
                    {
                        if(justHadCheck)
                        {
                            smInst->whoseTurn()->kingStatus.setFillColor(sf::Color(CHECK_COLOR,0));
                            smInst->whoseTurn()->kingStatus.setOutlineColor(sf::Color(CHECK_OUT_COLOR,0));
                        }
                        smInst->updateTurn();
                    }
                    smInst->whoseTurn()->stat.undercheck = smInst->underCheck(smInst->whoseTurn(), smInst->whoseTurn()->getKingPos());
                    if(smInst->whoseTurn()->stat.undercheck){
                        // std::cout << "under check opp player"<< std::endl;
                        justHadCheck = true;
                        smInst->whoseTurn()->kingStatus.setFillColor(sf::Color(CHECK_COLOR,255));
                        smInst->whoseTurn()->kingStatus.setOutlineColor(sf::Color(CHECK_OUT_COLOR,255));
                        // TODO Opposite king ki mkb check karo
                        // for(std::vector<Piece*>::iterator iter=smInst->kingAttacker.begin();iter!=smInst->kingAttacker.end();iter++)
                        // {
                        //     std::cout << (*iter)->getPieceType() << (*iter)->getPos().x << (*iter)->getPos().y << std::endl;
                        // }
                        // for(std::vector<sf::Vector2i>::iterator iter=smInst->smAttackSquares.begin();iter!=smInst->smAttackSquares.end();iter++)
                        // {
                        //     std::cout << "attack squares";
                        //     std::cout << iter->x << iter->y << std::endl;
                        //     // smInst->kingAttacker.clear();
                        // }
                        smInst->gameEnded = true;
                        for(std::vector<Piece*>::iterator iter = smInst->whoseTurn()->alive.begin(); iter!=smInst->whoseTurn()->alive.end(); iter++){
                            smInst->possibleSquaresList(smInst->whoseTurn()->getPlayerID(), (*iter)->getPieceType(), (*iter)->getPos(), tempPossibleSquares, *iter,true);
                            // std::cout << "possibleSquares size : " << tempPossibleSquares.size() << *vecKey((*iter)->getPos()) <<std::endl;
                            if(tempPossibleSquares.size() != 0)
                            {
                                smInst->gameEnded = false;
                                break;
                            }
                        }
                    }else
                    {
                        if(!justHadCheck)
                        {
                            smInst->whoseTurn()->kingStatus.setFillColor(sf::Color(CHECK_COLOR,0));
                            smInst->whoseTurn()->kingStatus.setOutlineColor(sf::Color(CHECK_OUT_COLOR,0));
                        }
                    }

                }
                    previousHeldPiece = nullptr;
                // else
                // {
                    for(std::vector<sf::Vector2i>::iterator iter = possibleSquares->begin(); iter!=possibleSquares->end();iter++ )
                    {
                        mcInst->setInvisible(iter->x, iter->y);
                    }
                // }
                // possibleSquares->clear();
            }
        }
        isMouseReleased = false;
        std::cout << "game ended : " << smInst->gameEnded << std::endl;
    }
    // std::cout << std::dec << (int)testBall.getPosition().x << std::endl;
    // testBall.setPosition(sf::Mouse::getPosition().x , sf::Mouse::getPosition().y );
}

void GameHandler::run(){
    sf::Clock frameRateClock;
    sf::Time timeSinceLastFrame = sf::Time::Zero;
    sf::Time frameTime = sf::seconds(1.0f / 60.f);
    while(mainWindow.isOpen()){
        timeSinceLastFrame += frameRateClock.restart();
        
        processInputs();
        while(timeSinceLastFrame > frameTime){
            timeSinceLastFrame -= frameTime;
            processInputs();
            update(frameTime);
        }
        render();
    }
}

void GameHandler::handleKeyInput(sf::Keyboard::Key key, bool state){
    switch(key)
    {
        case sf::Keyboard::W:
            // direction = (state ? (direction | 0x01) : (direction & ~0x01));
        break;

        case sf::Keyboard::A:
            // direction = (state ? (direction | 0x02) : (direction & ~0x02));
        break;

        case sf::Keyboard::S:
            // direction = (state ? (direction | 0x04) : (direction & ~0x04));

        break;

        case sf::Keyboard::D:
            // direction = (state ? (direction | 0x08) : (direction & ~0x08));
        break;
        case sf::Keyboard::Left:
            if(!state)
            {
                smInst->stepPast();
                for(std::vector<sf::Vector2i>::iterator iter = possibleSquares->begin(); iter!=possibleSquares->end();iter++ )
                {
                    // std::cout<< iter->x << " inv " << iter->y << std::endl;
                    mcInst->setInvisible(iter->x, iter->y);
                }
            // std::cout << "left key" << smInst->movesHist.size() << smInst->getTotalMoves() << smInst->getCurrentMove() << std::endl;

            }
            // direction = (state ? (direction | 0x08) : (direction & ~0x08));
        break;
        case sf::Keyboard::Right:
        if(!state)
        {
            smInst->stepFuture();
            // smInst->stepPast();
            // std::cout << "right key" << smInst->getTotalMoves() << smInst->getCurrentMove() << std::endl;

        }
            // direction = (state ? (direction | 0x08) : (direction & ~0x08));
        break;
    }
    // std::cout << std::hex << (int)direction << std::endl;

}

GameHandler::GameHandler(): mainWindow(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "We have Chess.com at home", 5U)
                                ,masterClock()
                                ,testBall(10.0f)
{
    testBall.setFillColor(sf::Color(2,128,233,45));
    testBall.setPosition( 0 , 0 );
    direction = 0 ;
    if(!testTexture.loadFromFile("assets/gengar.bmp"))
    {
        std::cerr << "Error loading Gengar texture! " << std::endl; 
    }

    player.setTexture(testTexture);
    player.scale(0.5, 0.5);
    // std::cout << "Player info: " << testTexture.getSize().x << testTexture.getSize().y << testTexture.getMaximumSize()<<  std::endl ; // see flush effect here by removing endl :)
    smInst->resetGame();
    // std::cout << *vecKey(sf::Vector2i(1,11))<< static_cast<char>(64)<<std::endl;
}


int main()
{
    GameHandler* mainHandler = new GameHandler();
    mainHandler->run();




    // sf::RenderWindow window(sf::VideoMode(-100, 0-100), "SFML works!");
    // sf::CircleShape shape(200.f);
    // shape.setFillColor(sf::Color(2,128,233,45));
    // sf::Clock samay;
    // sf::Time startTime = samay.getElapsedTime();
    // while (window.isOpen())
    // {
    //     sf::Event event;
    //     while (window.pollEvent(event))
    //     {
    //         if (event.type == sf::Event::Closed)
    //             window.close();
    //     }

    //     window.clear();
    //     shape.setRadius(0.3*samay.getElapsedTime().asMilliseconds());
    //     shape.setFillColor(sf::Color(0.3*samay.getElapsedTime().asMilliseconds(),128,233,45));
    //     window.draw(shape);
    //     window.display();
    // }

    return 0;
}


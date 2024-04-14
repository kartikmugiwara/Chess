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
        bool checkedPiece = false;
        Piece* heldPiece;
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
                std::cout << "Button Down : "<< sf::Mouse::getPosition(mainWindow).x << " " << sf::Mouse::getPosition(mainWindow).y << std::endl;
                isMousePressed = true;
                break;
            case sf::Event::MouseButtonReleased:
                std::cout << "Button Up : "<< sf::Mouse::getPosition(mainWindow).x << " " << sf::Mouse::getPosition(mainWindow).y << std::endl;
                isMousePressed = false;
                checkedPiece = false;
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
    
    testBall.move(movement * deltaTime.asSeconds());
    if(isMousePressed)
    {
        if( mousePos.x < 0)
            sf::Mouse::setPosition(sf::Vector2i(0,mousePos.y), mainWindow);
        if( mousePos.y < 0)
            sf::Mouse::setPosition(sf::Vector2i(mousePos.x,0), mainWindow);
        if( mousePos.x > WINDOW_SIZE)
            sf::Mouse::setPosition(sf::Vector2i(WINDOW_SIZE, mousePos.y ), mainWindow);
        if( mousePos.y > WINDOW_SIZE)
            sf::Mouse::setPosition(sf::Vector2i(mousePos.x, WINDOW_SIZE), mainWindow);

        mousePos = sf::Mouse::getPosition(mainWindow);

        if(!checkedPiece){
            heldPiece = smInst->getHeldRef(mousePos);
            checkedPiece = true;
        }
        if(heldPiece != nullptr)
        {
            heldPiece->updatePos(sf::Vector2i(1 + mousePos.x/(PIECE_SIZE + 2*PIECE_PAD), 1 + mousePos.y/(PIECE_SIZE + 2*PIECE_PAD)));
        }
        std::cout << "Button now : "<< mousePos.x << " " << mousePos.y << std::endl;
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
            direction = (state ? (direction | 0x01) : (direction & ~0x01));
        break;

        case sf::Keyboard::A:
            direction = (state ? (direction | 0x02) : (direction & ~0x02));
        break;

        case sf::Keyboard::S:
            direction = (state ? (direction | 0x04) : (direction & ~0x04));

        break;

        case sf::Keyboard::D:
            direction = (state ? (direction | 0x08) : (direction & ~0x08));
        break;
    }
    // std::cout << std::hex << (int)direction << std::endl;

}

GameHandler::GameHandler(): mainWindow(sf::VideoMode(8*(PIECE_SIZE + 2*PIECE_PAD), 8*(PIECE_SIZE + 2*PIECE_PAD)), "We have Chess.com at home", 5U)
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
    std::cout << "Player info: " << testTexture.getSize().x << testTexture.getSize().y << testTexture.getMaximumSize()<<  std::endl ; // see flush effect here by removing endl :)
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


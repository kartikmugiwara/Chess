#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string.h>
#include <assert.h>
#include <namespace.hpp>
#include <piece.hpp>
#include <costume.hpp>





// function that taks piece, current pos and spits possible squares?
// possible square()



class GameHandler{
    public:
        GameHandler();
        void run();
        CostumeHandler pieceCostHandler;
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
        Piece pawn;


    // protected:
};

void GameHandler::render(){
    mainWindow.clear();
    mainWindow.draw(testBall);
    mainWindow.draw(player);
    mainWindow.draw(*pawn.getSprite());
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
            // case sf::Event::MouseButtonPressed:
            //     std::cout << "Button Down : "<< sf::Mouse::getPosition().x << " " << sf::Mouse::getPosition().y << std::endl;
            //     break;
            case sf::Event::Closed:
                mainWindow.close();
                break;
        }
    }
}

void GameHandler::update(sf::Time deltaTime){
    // testBall.setRadius( (int)(masterClock.getElapsedTime().asMilliseconds() )% 255);
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

GameHandler::GameHandler(): mainWindow(sf::VideoMode(1000, 500), "PID Target System")
                                ,masterClock()
                                ,testBall(10.0f)
{
    pieceCostHandler.loadCostume(TextureID::pieceID::Pawn, "Pawn.png");
    testBall.setFillColor(sf::Color(2,128,233,45));
    testBall.setPosition( 0 , 0 );
    direction = 0 ;
    if(!testTexture.loadFromFile("assets/gengar.bmp"))
    {
        std::cerr << "Error loading Gengar texture! " << std::endl; 
    }

    player.setTexture(testTexture);
    pawn.setTexture(pieceCostHandler.getCostume(TextureID::pieceID::Pawn));
    player.scale(0.5, 0.5);
    std::cout << "Player info: " << testTexture.getSize().x << testTexture.getSize().y << testTexture.getMaximumSize()<<  std::endl ; // see flush effect here by removing endl :)
    
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


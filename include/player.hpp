#ifndef _HEADER_PLAYER
#define _HEADER_PLAYER

#include <namespace.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <gameconfig.hpp>
// #include <piece.hpp>
struct kingStat{
    bool move;
    bool kill;
    bool block;
    bool undercheck;
};

typedef struct kingStat kingStat;

class Piece; //Forward declaration
class Player{
    private:
        uint8_t playerID;
        sf::Time pRemTime;
        uint8_t direction;
        uint8_t score;
        uint8_t timeM, timeS;
        sf::Vector2i kingPos;
    public:
        Player(uint8_t ID, uint8_t dir, sf::Font& f){
            playerID = ID;
            direction = dir;
            scoreText.setFont(f);
            scoreText.setCharacterSize(24); // in pixels, not points!
            scoreText.setFillColor(sf::Color::Blue);
            scoreText.setStyle(sf::Text::Bold);

            timeText.setFont(f);
            timeText.setCharacterSize(40); // in pixels, not points!
            timeText.setFillColor(sf::Color::Red);
            timeText.setOutlineColor(sf::Color::Black);
            timeText.setOutlineThickness(2.0);
            timeText.setStyle(sf::Text::Bold);
            
            kingStatus.setFont(f);
            kingStatus.setCharacterSize(40); // in pixels, not points!
            kingStatus.setFillColor(sf::Color(255,0,0,100));
            kingStatus.setOutlineColor(sf::Color(255,255,255,100));
            kingStatus.setOutlineThickness(3.0);
            kingStatus.setStyle(sf::Text::Bold);
            
            scoreRect.setSize(sf::Vector2f(150.0, 50.0));
            scoreRect.setFillColor(sf::Color(255,255,255,30));
            pRemTime = sf::seconds(TOTAL_GAME_SEC);

            stat.undercheck = false;

        }

        std::vector<Piece*> alive{}, dead{};
        sf::Vector2i deathPos;
        sf::Clock pClock;

        sf::Text scoreText, timeText, kingStatus;
        sf::RectangleShape scoreRect;
        kingStat stat{false, false, false,false};

        uint8_t getDirection();
        uint8_t getScore();
        uint8_t getTime(bool ms); //bool 1 m, 0 s
        sf::Time getRemTime();
        uint8_t getPlayerID();
        void setScore(uint8_t newScore);
        void updateRemTime();
        sf::Vector2i getKingPos();
        void setKingPos(sf::Vector2i newPos);


};



#endif

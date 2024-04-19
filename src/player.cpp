#include <player.hpp>

sf::Time Player::getRemTime(){
    return pRemTime;
}

void Player::updateRemTime(sf::Time time){
    pRemTime += time;
}

uint8_t Player::getPlayerID(){
    return playerID;
}

uint8_t Player::getDirection(){
    return direction;
}

uint8_t Player::getScore(){
    return score;
}

void Player::setScore(uint8_t newScore){
    score = newScore;
}
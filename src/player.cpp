#include <player.hpp>
#include <iostream>
sf::Time Player::getRemTime(){
    return pRemTime;
}

void Player::updateRemTime(){
    pRemTime -= pClock.getElapsedTime();
    pClock.restart();
    timeM = ((uint16_t)pRemTime.asSeconds())/60;
    timeS = (uint16_t)(pRemTime.asSeconds())%60;
    std::string sec_str = std::to_string(timeS);
    if (sec_str.size()==1)
        sec_str = '0' + sec_str;
    std::string min_str = std::to_string(timeM);
    if (min_str.size()==1)
        min_str = '0' + min_str;
    timeText.setString(min_str + " : " + sec_str);
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

uint8_t Player::getTime(bool ms) //bool 1 m, 0 s
{
    return ms ? timeM : timeS;
}

sf::Vector2i Player::getKingPos(){
    return kingPos;
}

void Player::setKingPos(sf::Vector2i newPos){
    kingPos = newPos;
}

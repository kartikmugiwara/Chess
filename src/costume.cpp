#include <costume.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string.h>
#include <assert.h>

bool CostumeHandler::loadCostume(TextureID::pieceName id, const std::string& filename){
    std::unique_ptr<sf::Texture> newTexture(new sf::Texture());
    bool ret = newTexture->loadFromFile("assets/" + filename);
    if(!ret)
        throw std::runtime_error("TextureHolder::load - Failed to load "
                    + filename);
    std::pair< std::map<TextureID::pieceName, std::unique_ptr<sf::Texture>>::iterator , bool > returnPair = textureMap.insert(std::make_pair(id, std::move(newTexture)));
    assert(returnPair.second);
    
    return ret;
}

sf::Texture& CostumeHandler::getCostume(TextureID::pieceName id){
    std::map<TextureID::pieceName, std::unique_ptr<sf::Texture>>::iterator textureIter = textureMap.find(id);
    assert(textureIter!=textureMap.end());
    return *textureIter->second; 
}
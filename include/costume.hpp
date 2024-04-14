#ifndef __INCLUDE_COSTUME__
#define __INCLUDE_COSTUME__
#include <namespace.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string.h>
#include <assert.h>

class CostumeHandler{
public:
bool loadCostume(TextureID::pieceName id, const std::string& filename);
sf::Texture& getCostume(TextureID::pieceName id);

private:

std::map<TextureID::pieceName, std::unique_ptr<sf::Texture> > textureMap;


};


#endif


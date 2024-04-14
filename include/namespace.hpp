#ifndef __MY_NAMESPACE__
#define __MY_NAMESPACE__
#include <string>
#include <SFML/System.hpp>
namespace TextureID{
    enum pieceName {Pawn, Bishop, Knight, Rook, Queen, King, TOTAL_PIECES}; 
}

// std::string* vecKey(sf::Vector2i vec){// limited to 26 columncs
//     static std::string key = "" ;
//     key = vec.x + 64;
//     uint16_t a = vec.y;
//     while(a/10 !=0){
//         key += static_cast<char>(a%10);
//         a= a/10;
//     }
//     key += a%10;
//     std::reverse(key.begin(),key.end());
//     return &key;
// }

#endif
#pragma once
#include <SFML/Graphics.hpp>

// Item class used for each item in the game
class Item{

public:
    sf::Texture *texture;
    sf::Sprite  *sprite;
    sf::String  name;
    int         id;
    int         price = 0;
    char        tag;    // tags : s(shopItem) i(inventoryItem)
    float       priceModifier;
    
    // generate a copy of the class
    Item Clone();
};
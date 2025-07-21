#include <SFML/Graphics.hpp>
#include "Item.h"

// Shop class for randomly generating new shops
class Shop{

public:
    sf::Texture *texture;
    sf::Sprite  *sprite;
    std::vector<Item> items;
    int currentHigh;    // item being bought at a high price
    int currentLow; // item being bought at a low price
    int forecastHigh;    // item being bought at a high price in next shop
    int forecastLow; // item being bought at a low price in next shop
    int lowestItemPrice;    // lowest item price in the shop
};
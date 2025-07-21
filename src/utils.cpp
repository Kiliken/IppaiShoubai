#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <random>
#include <numeric>
#include <string>
#include "Item.cpp"
#include "Shop.cpp"

#define ASSETS(file) ("../assets/" ## file)

std::vector<Item> ALL_ITEMS;    // vector of all items in the game

namespace utils {

int GetRandomIntInRange(int min, int max) {
    static std::random_device rd;  // only used once to initialize (seed) engine
    static std::mt19937 gen(rd()); // static to keep the same generator between calls

    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

    // set all default items in a vector
std::vector<Item> SetUpItems() {
    std::vector<Item> items;

    Item Bow;
    Bow.texture = new sf::Texture();
    Bow.texture->loadFromFile(ASSETS("bow.png"));
    Bow.sprite = new sf::Sprite(*Bow.texture);
    Bow.sprite->setOrigin(sf::Vector2f(0.f, 5.f));
    Bow.name = "Bow";
    Bow.id = 0;
    Bow.price = 40;
    Bow.tag = 's';
    items.push_back(Bow);

    Item HpPotion;
    HpPotion.texture = new sf::Texture();
    HpPotion.texture->loadFromFile(ASSETS("hppotion.png"));
    HpPotion.sprite = new sf::Sprite(*HpPotion.texture);
    HpPotion.name = "Heal Potion";
    HpPotion.id = 1;
    HpPotion.price = 20;
    HpPotion.tag = 's';
    items.push_back(HpPotion);

    Item ManaPotion;
    ManaPotion.texture = new sf::Texture();
    ManaPotion.texture->loadFromFile(ASSETS("manapotion.png"));
    ManaPotion.sprite = new sf::Sprite(*ManaPotion.texture);
    ManaPotion.name = "Mana Potion";
    ManaPotion.id = 2;
    ManaPotion.price = 30;
    ManaPotion.tag = 's';
    items.push_back(ManaPotion);
    
    Item Sword;
    Sword.texture = new sf::Texture();
    Sword.texture->loadFromFile(ASSETS("psword.png"));
    Sword.sprite = new sf::Sprite(*Sword.texture);
    Sword.name = "Paola Sword";
    Sword.id = 3;
    Sword.price = 90;
    Sword.tag = 's';
    items.push_back(Sword);

    Item Shield;
    Shield.texture = new sf::Texture();
    Shield.texture -> loadFromFile(ASSETS("pshield.png"));
    Shield.sprite = new sf::Sprite(*Shield.texture);
    Shield.sprite->setOrigin(sf::Vector2f(0.f, 5.f));
    Shield.name = "Paolo Shield";
    Shield.id = 4;
    Shield.price = 70;
    Shield.tag = 's';
    items.push_back(Shield);
    
    return items;
}


// int for item index in all items vector, -1 for random
Shop SetUpShop(int highItem, int lowItem){
    Shop shop;

    // SHOP BACKGROUND
    shop.texture = new sf::Texture();

    int bg = rand() % 5;
    switch(bg){
        case 0:
            shop.texture->loadFromFile(ASSETS("background.png"));
            break;
        case 1:
            shop.texture->loadFromFile(ASSETS("background1.png"));
            break;
        case 2:
            shop.texture->loadFromFile(ASSETS("background2.png"));
            break;
        case 3:
            shop.texture->loadFromFile(ASSETS("background3.png"));
            break;
        case 4:
            shop.texture->loadFromFile(ASSETS("background4.png"));
            break;
    }
    //shop.texture->loadFromFile("../assets/background.png");
    shop.sprite = new sf::Sprite(*shop.texture);

    shop.sprite->setOrigin({0.f,0.f});
    shop.sprite->setScale({1.25f,1.25f});


    // SHOP ITEMS
    std::vector<int> indices(ALL_ITEMS.size()); // Shuffle item indices to ensure uniqueness
    std::iota(indices.begin(), indices.end(), 0); // Fill with 0 to ALL_ITEMS.size()-1
    std::shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device{}()));


    
    shop.lowestItemPrice = 5000;
    // add random items to the vector
    // 3 ITEMS, INCRESE TO 4 LATER
    for(int i = 0; i < 4 && i < indices.size(); i++){
        int itemNo = indices[i];
        Item item = ALL_ITEMS[itemNo].Clone();

        if(item.price < shop.lowestItemPrice)
            shop.lowestItemPrice = item.price;

        item.sprite->setScale({0.10f, 0.10f});
        shop.items.push_back(item);
    }


    // SET PRICES
    if(highItem != -1){
        shop.currentHigh = highItem;
    }
    if(lowItem != -1){
        shop.currentLow = lowItem;
    }
    shop.forecastHigh = shop.forecastLow = rand() % ALL_ITEMS.size();
    std::cout << shop.forecastHigh << std::endl;
    while (shop.forecastHigh == shop.forecastLow)
    {
        shop.forecastLow = rand() % ALL_ITEMS.size();
    }
    std::cout << shop.forecastLow << std::endl;
    

    return shop;
}


// modify the price modifier in the main item list each round
void ModifyPrices(int highItem, int lowItem){
    for (int i = 0; i < ALL_ITEMS.size(); i++)
    {
        float modifier = GetRandomIntInRange(1, 7) * 0.1f;
        if(i == highItem){
            ALL_ITEMS[i].priceModifier = modifier;
        }
        else if (i == lowItem)
        {
            ALL_ITEMS[i].priceModifier = -modifier;
        }
        else{
            if(GetRandomIntInRange(0, 1) == 0){
                ALL_ITEMS[i].priceModifier = modifier;
            }
            else{
                ALL_ITEMS[i].priceModifier = -modifier;
            }
        }
        std::cout << ALL_ITEMS[i].name.toAnsiString() + " " + std::to_string(ALL_ITEMS[i].priceModifier) << std::endl;
    }
}


// modify the prices of items in store and inventory
void ModifyItemListPrice(std::vector<Item> &items){
    for(int i = 0; i < items.size(); i++){
        items[i].price = ALL_ITEMS[items[i].id].price + ALL_ITEMS[items[i].id].price * ALL_ITEMS[items[i].id].priceModifier;
        std::cout << items[i].name.toAnsiString() + " " + std::to_string(items[i].price) << std::endl;
    }
}


void FindLowestPriceItem(Shop &shop){
    shop.lowestItemPrice = 5000;
    // add random items to the vector
    // 3 ITEMS, INCRESE TO 4 LATER
    for(int i = 0; i < shop.items.size(); i++){
        if(shop.items[i].price < shop.lowestItemPrice)
            shop.lowestItemPrice = shop.items[i].price;
    }
}


// get the item being clicked on
Item InteractWith(std::vector<Item> &items, sf::Sprite mousePos){
    Item empty;
    empty.sprite = nullptr;

    for(int i = 0; i < items.size(); i++){
        if (items[i].sprite->getGlobalBounds().findIntersection(mousePos.getGlobalBounds())){
        return items[i];
        }    
    }

    return empty;
}

}
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>

#define ASSETS "../assets/"

// Item struct used for each item in the game
struct Item {
    sf::Texture *texture;
    sf::Sprite  *sprite;
    sf::String  name;
    int         price = 0;
    char        tag;    // tags : s(shopItem) i(inventoryItem)
    
    // generate a copy of the struct
    Item Clone() {
        Item newItem;
        newItem.texture = this->texture;
        newItem.sprite = new sf::Sprite(*newItem.texture);
        newItem.sprite->setScale(this->sprite->getScale());
        newItem.price = this->price;
        newItem.name = this->name;
        newItem.tag = this->tag;

        return newItem;
    }
};

struct Shop{
    sf::Texture *texture;
    sf::Sprite  *sprite;
    std::vector<Item> items;
    Item forecastHigh;    // item being bought at a high price in next shop
    Item forecastLow; // item being bought at a low price in next shop
};

std::vector<Item> ALL_ITEMS;    // vector of all items in the game


std::vector<Item> SetUpItems(); // set all default items in a vector
Shop SetUpShop(bool first);
void GoToNextShop();


int main(){
    srand(static_cast<unsigned int>(time(0)));  // random seed
    int money = 100;
    int moneyGoal = 1000;
    int scene = 0;  // current shop

    // WINDOW
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Ippai Shoubai");
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(60);

    // cursor
    const sf::Texture cursorText("../assets/cursor.png");
    sf::Sprite cursorSprite(cursorText);
    cursorSprite.setOrigin({cursorText.getSize().x / 2.f,cursorText.getSize().y/ 2.f});
    cursorSprite.setScale({0.06f,0.06f});

    ALL_ITEMS = SetUpItems();
    //std::cout << ALL_ITEMS[3].name.toAnsiString() << std::endl;

    Shop currentShop = SetUpShop(true);

    // MAIN LOOP
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }


        // cursor
        cursorSprite.setPosition({static_cast<float>(sf::Mouse::getPosition().x) - window.getPosition().x,static_cast<float>(sf::Mouse::getPosition().y) - window.getPosition().y});
        
        // RENDERING
        window.clear();
        window.draw(*currentShop.sprite);
        window.draw(cursorSprite);
        window.display();
    }
}


// set all default items in a vector
std::vector<Item> SetUpItems() {
    std::vector<Item> items;

    Item Bow;
    Bow.texture = new sf::Texture();
    Bow.texture->loadFromFile("../assets/bow.png");
    Bow.sprite = new sf::Sprite(*Bow.texture);
    Bow.name = "Bow";
    Bow.price = 10;
    Bow.tag = 's';
    items.push_back(Bow);

    Item HpPotion;
    HpPotion.texture = new sf::Texture();
    HpPotion.texture->loadFromFile("../assets/hppotion.png");
    HpPotion.sprite = new sf::Sprite(*HpPotion.texture);
    HpPotion.name = "Heal Potion";
    HpPotion.price = 20;
    HpPotion.tag = 's';
    items.push_back(HpPotion);

    Item ManaPotion;
    ManaPotion.texture = new sf::Texture();
    ManaPotion.texture->loadFromFile("../assets/manapotion.png");
    ManaPotion.sprite = new sf::Sprite(*ManaPotion.texture);
    ManaPotion.name = "Mana Potion";
    ManaPotion.price = 40;
    ManaPotion.tag = 's';
    items.push_back(ManaPotion);
    
    Item Sword;
    Sword.texture = new sf::Texture();
    Sword.texture->loadFromFile("../assets/psword.png");
    Sword.sprite = new sf::Sprite(*Sword.texture);
    Sword.name = "Paolo Sword";
    Sword.price = 100;
    Sword.tag = 's';
    items.push_back(Sword);
    
    return items;
}


Shop SetUpShop(bool first){
    Shop shop;

    shop.texture = new sf::Texture();

    int bg = rand() % 3;
    switch(bg){
        case 0:
            shop.texture->loadFromFile("../assets/background.png");
            break;
        case 1:
            shop.texture->loadFromFile("../assets/background1.png");
            break;
        case 2:
            shop.texture->loadFromFile("../assets/background2.png");
            break;
    }
    //shop.texture->loadFromFile("../assets/background.png");
    shop.sprite = new sf::Sprite(*shop.texture);

    shop.sprite->setOrigin({0.f,0.f});
    shop.sprite->setScale({1.25f,1.25f});

    // add random items to the vector
    for(int i = 0; i < 8; i++){
        shop.items.push_back(ALL_ITEMS[rand() % ALL_ITEMS.size()].Clone());
        shop.items[i].sprite->setScale({0.10f,0.10f});
    }

    return shop;
}
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


// set all default items in a vector and 
std::vector<Item> DeclareItems() {
    std::vector<Item> items;    // vector of random items in shop
    std::vector<Item> itemPull; // vector of default items

    Item Bow;
    Bow.texture = new sf::Texture();
    Bow.texture->loadFromFile("../assets/bow.png");
    Bow.sprite = new sf::Sprite(*Bow.texture);
    Bow.name = "Bow";
    Bow.price = 10;
    Bow.tag = 's';
    itemPull.push_back(Bow);

    Item HpPotion;
    HpPotion.texture = new sf::Texture();
    HpPotion.texture->loadFromFile("../assets/hppotion.png");
    HpPotion.sprite = new sf::Sprite(*HpPotion.texture);
    HpPotion.name = "Heal Potion";
    HpPotion.price = 20;
    HpPotion.tag = 's';
    itemPull.push_back(HpPotion);

    Item ManaPotion;
    ManaPotion.texture = new sf::Texture();
    ManaPotion.texture->loadFromFile("../assets/manapotion.png");
    ManaPotion.sprite = new sf::Sprite(*ManaPotion.texture);
    ManaPotion.name = "Mana Potion";
    ManaPotion.price = 40;
    ManaPotion.tag = 's';
    itemPull.push_back(ManaPotion);
    
    Item Sword;
    Sword.texture = new sf::Texture();
    Sword.texture->loadFromFile("../assets/psword.png");
    Sword.sprite = new sf::Sprite(*Sword.texture);
    Sword.name = "Paolo Sword";
    Sword.price = 100;
    Sword.tag = 's';
    itemPull.push_back(Sword);
    

    // add random items to the vector
    for(int i = 0; i < 8; i++){
        items.push_back(itemPull[rand() % itemPull.size()].Clone());
        items[i].sprite->setScale({0.10f,0.10f});
    }

    return items;
}


// get the item being clicked on
Item InteractWith(std::vector<Item> items, sf::Sprite mousePos){
    Item empty;

    for(int i = 0; i < items.size(); i++){
        if (items[i].sprite->getGlobalBounds().findIntersection(mousePos.getGlobalBounds())){
        return items[i];
        }    
    }

    return empty;
}


int main()
{
    srand(static_cast<unsigned int>(time(0)));  // random seed
    int money = 100;
    bool mouseLeftDown = false;
    int scene = 0;  // current shop

    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Ippai Shoubai");
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(60);
    //const sf::Image gameIcon("../assets/cursor.png");
    //window.setIcon(gameIcon);

    // money text
    const sf::Font font("../assets/arial.ttf");
    sf::Text moneyText(font, std::to_string(money) + "$");
    moneyText.setCharacterSize(30);
    moneyText.setStyle(sf::Text::Bold);
    moneyText.setFillColor(sf::Color::Black);
    moneyText.setPosition({1150.f,200.f});

    // text for item being hovered on
    sf::Text hoverText(font,"");
    hoverText.setCharacterSize(40);
    hoverText.setStyle(sf::Text::Bold);
    hoverText.setFillColor(sf::Color::Black);
    hoverText.setPosition({200.f,650.f});

    // item positions
    std::vector<Item> inventory;
    sf::Vector2f inventoryPos[12] = {
        {925.f,275.f},{1025.f,275.f},{1125.f,275.f},
        {925.f,375.f},{1025.f,375.f},{1125.f,375.f},
        {925.f,475.f},{1025.f,475.f},{1125.f,475.f},
        {925.f,575.f},{1025.f,575.f},{1125.f,575.f},
    };

    sf::Vector2f storePos[8] = {
        {50.f,50.f},{200.f,50.f},
        {50.f,200.f},{200.f,200.f},
        {50.f,350.f},{200.f,350.f},
        {50.f,500.f},{200.f,500.f},
    };

    // cursor
    const sf::Texture cursorText("../assets/cursor.png");
    sf::Sprite cursorSprite(cursorText);
    cursorSprite.setOrigin({cursorText.getSize().x / 2.f,cursorText.getSize().y/ 2.f});
    cursorSprite.setScale({0.06f,0.06f});

    // backgrounds of shops
    const sf::Texture bgText("../assets/background.png");
    std::vector<sf::Sprite> bgSprite;
    bgSprite.push_back(sf::Sprite(bgText));
    bgSprite[0].setOrigin({0.f,0.f});
    bgSprite[0].setScale({1.25f,1.25f});
    bgSprite.push_back(sf::Sprite(bgText));
    bgSprite[1].setOrigin({0.f,0.f});
    bgSprite[1].setScale({1.25f,1.25f});
    bgSprite[1].setColor(sf::Color::Red);
    bgSprite.push_back(sf::Sprite(bgText));
    bgSprite[2].setOrigin({0.f,0.f});
    bgSprite[2].setScale({1.25f,1.25f});
    bgSprite[2].setColor(sf::Color::Blue);

    // arrows for switching shops
    const sf::Texture arrowsText("../assets/arrow.png");
    sf::Sprite leftArrow(arrowsText);
    leftArrow.setOrigin({arrowsText.getSize().x / 2.f,arrowsText.getSize().y/ 2.f});
    leftArrow.setScale({0.25f,0.25f});
    leftArrow.setPosition({1050.f,100.f});
    leftArrow.setRotation(sf::Angle(sf::degrees(180)));
    sf::Sprite rightArrow(arrowsText);
    rightArrow.setOrigin({arrowsText.getSize().x / 2.f,arrowsText.getSize().y/ 2.f});
    rightArrow.setScale({0.25f,0.25f});
    rightArrow.setPosition({1200.f,100.f});
    
    // vectors of items, in a vector for each shop
    std::vector<std::vector<Item>> items;
    items.push_back(DeclareItems());
    items.push_back(DeclareItems());
    items.push_back(DeclareItems());

    Item itemClicked;
    Item itemHover;


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // close window
            if (event->is<sf::Event::Closed>())
                window.close();

            // mouse button press
            if (event->is<sf::Event::MouseButtonPressed>()){
                itemClicked = InteractWith(items[scene],cursorSprite).Clone();

                // buy item from shop
                if(inventory.size() < 12 && money >= itemClicked.price && itemClicked.tag == 's') {
                    itemClicked.tag = 'i';
                    //itemClicked.sprite->setColor(sf::Color(rand() % 255,rand() % 255,rand() % 255));
                    money -= itemClicked.price;
                    moneyText.setString(std::to_string(money) + "$");
                    inventory.push_back(itemClicked);
                    break;
                    //std::cout << money << std::endl;
                }
                // sell item from inventory
                else{
                    itemClicked = InteractWith(inventory,cursorSprite);
                
                    if(itemClicked.tag == 'i'){
                        for(int i=0; i < inventory.size(); i++){
                            if(itemClicked.sprite->getPosition() == inventory[i].sprite->getPosition()){
                                money += itemClicked.price;
                                moneyText.setString(std::to_string(money) + "$");
                                inventory.erase(inventory.begin() + i);
                                break;
                            }
                        }
                    }
                }

                // switch shops
                if (leftArrow.getGlobalBounds().findIntersection(cursorSprite.getGlobalBounds()) && scene > 0){
                    scene--;
                    break;
                }
                if (rightArrow.getGlobalBounds().findIntersection(cursorSprite.getGlobalBounds()) && scene < 2){
                    scene++;
                    break;
                }

            }
        }

        // corsor
        cursorSprite.setPosition({static_cast<float>(sf::Mouse::getPosition().x) - window.getPosition().x,static_cast<float>(sf::Mouse::getPosition().y) - window.getPosition().y});
        
        // set item positions
       for(int i = 0; i < inventory.size(); i++){
            inventory[i].sprite->setPosition(inventoryPos[i]);
            //inventory[i].sprite->setScale({0.10f,0.10f});
        }

        for(int i = 0; i < items[scene].size(); i++){
            items[scene][i].sprite->setPosition(storePos[i]);
            
        }
        
        // set item text
        {
            hoverText.setString("");
            itemHover = InteractWith(items[scene],cursorSprite);
            if(itemHover.price != 0){
                hoverText.setString(itemHover.name + " : " + std::to_string(itemHover.price) + "$");
            }else{
                itemHover = InteractWith(inventory,cursorSprite);
                if(itemHover.price != 0)
                    hoverText.setString(itemHover.name + " : " + std::to_string(itemHover.price) + "$");
            }
        }

        window.clear();
        window.draw(bgSprite[scene]);
        //window.draw(shape);
        

        for(int i = 0; i < items[scene].size(); i++)
            window.draw(*items[scene][i].sprite);

        for(int i = 0; i < inventory.size(); i++)
            window.draw(*inventory[i].sprite);


        window.draw(hoverText);
        window.draw(moneyText);
        window.draw(leftArrow);
        window.draw(rightArrow);
        window.draw(cursorSprite);
        
        window.display();
    }
}


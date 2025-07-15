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

// Shop struct for randomly generating new shops
struct Shop{
    sf::Texture *texture;
    sf::Sprite  *sprite;
    std::vector<Item> items;
    int currentHigh;    // item being bought at a high price
    int currentLow; // item being bought at a low price
    int forecastHigh;    // item being bought at a high price in next shop
    int forecastLow; // item being bought at a low price in next shop
    int lowestItemPrice;    // lowest item price in the shop
};


std::vector<Item> ALL_ITEMS;    // vector of all items in the game
float HIGH_RATE = 0.8f; // multiplier for the items being bought with a high price in the shop (+80%)
float HIGH_RATE_SHOP = 2.f; // multiplier for high price items being sold in the shop (2x)
float LOW_RATE = 0.5f;  // low price multiplier


// FORWARD DECLARATIONS
std::vector<Item> SetUpItems(); // set all default items in a vector
Shop SetUpShop(int highItem, int lowItem);
Item InteractWith(std::vector<Item> items, sf::Sprite mousePos); // mouse interaction for items



int main(){
    srand(static_cast<unsigned int>(time(0)));  // random seed
    int money = 100;
    int moneyGoal = 1000;
    int scene = 0;  // current shop
    bool madeTransaction = false;   // bought or sold something
    bool gameClear = false;
    bool gameOver = false;

    // WINDOW
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Ippai Shoubai");
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(60);

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

    // game clear and game over texts
    sf::Text GameClearText(font,"GAME CLEAR");
    GameClearText.setCharacterSize(80);
    GameClearText.setStyle(sf::Text::Bold);
    GameClearText.setFillColor(sf::Color::Green);
    GameClearText.setPosition({350.f,300.f});
    sf::Text GameOverText(font,"GAME OVER");
    GameOverText.setCharacterSize(80);
    GameOverText.setStyle(sf::Text::Bold);
    GameOverText.setFillColor(sf::Color::Red);
    GameOverText.setPosition({350.f,300.f});

    // cursor
    const sf::Texture cursorText("../assets/cursor.png");
    sf::Sprite cursorSprite(cursorText);
    cursorSprite.setOrigin({cursorText.getSize().x / 2.f,cursorText.getSize().y/ 2.f});
    cursorSprite.setScale({0.06f,0.06f});

    // arrow for switching shops
    const sf::Texture arrowsText("../assets/arrow.png");
    sf::Sprite rightArrow(arrowsText);
    rightArrow.setOrigin({arrowsText.getSize().x / 2.f,arrowsText.getSize().y/ 2.f});
    rightArrow.setScale({0.25f,0.25f});
    rightArrow.setPosition({1200.f,100.f});

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

    ALL_ITEMS = SetUpItems();
    //std::cout << ALL_ITEMS[3].name.toAnsiString() << std::endl;
    Item itemClicked;
    Item itemHover;
    Item itemHigh;  // high price item forecast
    Item itemLow;   // low price item forecast

    // Set up first shop
    Shop currentShop = SetUpShop(-1, -1);
    itemHigh = ALL_ITEMS[currentShop.forecastHigh].Clone();
    itemHigh.sprite->setScale({0.10f,0.10f});
    itemHigh.sprite->setPosition({500.f, 100.f});
    itemLow = ALL_ITEMS[currentShop.forecastLow].Clone();
    itemLow.sprite->setScale({0.10f,0.10f});
    itemLow.sprite->setPosition({600.f, 100.f});

    // MAIN LOOP
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // mouse button press
            if (event->is<sf::Event::MouseButtonPressed>() && !gameClear && !gameOver){
                itemClicked = InteractWith(currentShop.items,cursorSprite).Clone();

                // double price in shop if high price item
                int buyPrice = itemClicked.price;
                if(itemClicked.name == ALL_ITEMS[currentShop.currentHigh].name){
                    buyPrice = itemClicked.price * HIGH_RATE_SHOP;
                }
                // else if(itemClicked.name == ALL_ITEMS[currentShop.currentLow].name){
                //     //std::cout << "low item" << std::endl;
                //     buyPrice = itemClicked.price - itemClicked.price * LOW_RATE;
                // }

                // buy item from shop
                if(inventory.size() < 12 && money >= buyPrice && itemClicked.tag == 's') {
                    itemClicked.tag = 'i';
                    //itemClicked.sprite->setColor(sf::Color(rand() % 255,rand() % 255,rand() % 255));
                    money -= buyPrice;
                    moneyText.setString(std::to_string(money) + "$");
                    inventory.push_back(itemClicked);
                    madeTransaction = true;
                    break;
                    //std::cout << money << std::endl;
                }
                // sell item from inventory
                else{
                    itemClicked = InteractWith(inventory,cursorSprite);
                
                    if(itemClicked.tag == 'i'){
                        for(int i=0; i < inventory.size(); i++){
                            if(itemClicked.sprite->getPosition() == inventory[i].sprite->getPosition()){
                                if(itemClicked.name == ALL_ITEMS[currentShop.currentHigh].name){
                                    //std::cout << "high item" << std::endl;
                                    money += itemClicked.price + itemClicked.price * HIGH_RATE;
                                }
                                else if(itemClicked.name == ALL_ITEMS[currentShop.currentLow].name){
                                    //std::cout << "low item" << std::endl;
                                    money += itemClicked.price - itemClicked.price * LOW_RATE;
                                }
                                else{
                                    money += itemClicked.price;
                                }
                                //money += itemClicked.price;
                                
                                moneyText.setString(std::to_string(money) + "$");
                                inventory.erase(inventory.begin() + i);

                                madeTransaction = true;
                                break;
                            }
                        }
                    }
                }

                // go to next shop
                if (rightArrow.getGlobalBounds().findIntersection(cursorSprite.getGlobalBounds()) && madeTransaction){
                    currentShop = SetUpShop(currentShop.forecastHigh, currentShop.forecastLow);

                    itemHigh = ALL_ITEMS[currentShop.forecastHigh].Clone();
                    itemHigh.sprite->setScale({0.10f,0.10f});
                    itemHigh.sprite->setPosition({500.f, 100.f});

                    itemLow = ALL_ITEMS[currentShop.forecastLow].Clone();
                    itemLow.sprite->setScale({0.10f,0.10f});
                    itemLow.sprite->setPosition({600.f, 100.f});

                    scene++;
                    madeTransaction = false;
                    break;
                }
            }
        }

        // win lose condition
        if(money >= moneyGoal){
            gameClear = true;
        }
        else if(money < currentShop.lowestItemPrice && inventory.size() == 0){
            gameOver = true;
        }
        

        // set item positions
        for(int i = 0; i < inventory.size(); i++){
            inventory[i].sprite->setPosition(inventoryPos[i]);
            //inventory[i].sprite->setScale({0.10f,0.10f});
        }
        for(int i = 0; i < currentShop.items.size(); i++){
            currentShop.items[i].sprite->setPosition(storePos[i]);
            
        }

        // cursor
        cursorSprite.setPosition({static_cast<float>(sf::Mouse::getPosition().x) - window.getPosition().x,static_cast<float>(sf::Mouse::getPosition().y) - window.getPosition().y});
        
        // text
        hoverText.setString("");
        itemHover = InteractWith(currentShop.items, cursorSprite);
        if(itemHover.price != 0){
            if(itemHover.name == ALL_ITEMS[currentShop.currentHigh].name){
                hoverText.setString(itemHover.name + " : " + std::to_string(static_cast<int>(itemHover.price * HIGH_RATE_SHOP)) + "$");
            }
            // else if(itemHover.name == ALL_ITEMS[currentShop.currentLow].name){
            //     hoverText.setString(itemHover.name + " : " + std::to_string(static_cast<int>(itemHover.price - itemHover.price * LOW_RATE)) + "$");
            // }
            else{
                hoverText.setString(itemHover.name + " : " + std::to_string(itemHover.price) + "$");
            }
            //hoverText.setString(itemHover.name + " : " + std::to_string(itemHover.price) + "$");
        }else{
            itemHover = InteractWith(inventory, cursorSprite);
            if(itemHover.price != 0){
                if(itemHover.name == ALL_ITEMS[currentShop.currentHigh].name){
                    hoverText.setString(itemHover.name + " : " + std::to_string(static_cast<int>(itemHover.price + itemHover.price * HIGH_RATE)) + "$");
                }
                else if(itemHover.name == ALL_ITEMS[currentShop.currentLow].name){
                    hoverText.setString(itemHover.name + " : " + std::to_string(static_cast<int>(itemHover.price - itemHover.price * LOW_RATE)) + "$");
                }
                else{
                    hoverText.setString(itemHover.name + " : " + std::to_string(itemHover.price) + "$");
                }
                //hoverText.setString(itemHover.name + " : " + std::to_string(itemHover.price) + "$");
                
            }
        }
        

        // RENDERING
        window.clear();
        window.draw(*currentShop.sprite);

        for(int i = 0; i < inventory.size(); i++)
            window.draw(*inventory[i].sprite);
        for(int i = 0; i < currentShop.items.size(); i++)
            window.draw(*currentShop.items[i].sprite);

        if(itemHigh.price != 0)
            window.draw(*itemHigh.sprite);
        if(itemLow.price != 0)
            window.draw(*itemLow.sprite);

        window.draw(hoverText);
        window.draw(moneyText);
        if(madeTransaction)
            window.draw(rightArrow);

        if(gameClear)
            window.draw(GameClearText);
        if(gameOver)
            window.draw(GameOverText);

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


// int for item index in all items vector, -1 for random
Shop SetUpShop(int highItem, int lowItem){
    Shop shop;

    // SHOP BACKGROUND
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


    // SHOP ITEMS
    shop.lowestItemPrice = 5000;
    // add random items to the vector
    for(int i = 0; i < 4; i++){
        // // item being bought with high price
        // if(i == 0 && highItem != -1){
        //     Item h = ALL_ITEMS[highItem].Clone();
        //     h.price = h.price + h.price * HIGH_RATE;
        //     std::cout << h.price << std::endl;
        //     shop.items.push_back(h);
        // }
        // // item being bought with low price
        // else if(i == 1 && lowItem != -1){
        //     Item l = ALL_ITEMS[lowItem].Clone();
        //     l.price = l.price - l.price * LOW_RATE;
        //     std::cout << l.price << std::endl;
        //     shop.items.push_back(l);
        // }
        // // item being bought with high price
        // else{
        //     shop.items.push_back(ALL_ITEMS[rand() % ALL_ITEMS.size()].Clone());
        // }

        int itemNo = rand() % ALL_ITEMS.size();
        Item item = ALL_ITEMS[itemNo].Clone();
        // item being bought with high price
        if(itemNo == highItem){
            item.price = item.price + item.price * HIGH_RATE;
        }
        // // item being bought with low price
        // else if(itemNo == lowItem){
        //     item.price = item.price - item.price * LOW_RATE;
        // }
        if(item.price < shop.lowestItemPrice)
            shop.lowestItemPrice = item.price;

        shop.items.push_back(item);
        
        //shop.items.push_back(ALL_ITEMS[rand() % ALL_ITEMS.size()].Clone());
        shop.items[i].sprite->setScale({0.10f,0.10f});
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